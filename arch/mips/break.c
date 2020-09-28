#include "break.h"
#include "capstone/capstone.h"
#include "util.h" /* VERBOSE_OUTPUT*/

#include <assert.h>   /* assert */
#include <stdint.h>   /* uint32_t */
#include <stdio.h>    /* perror */
#include <sys/mman.h> /* mprotect */
#include <unistd.h>   /* getpagesize */

#define INSTR_LEN 4 /* in byte */
#define INSTR_ADDR_MASK (ADDR)(~(INSTR_LEN - 1))

typedef uint32_t MIPSInst;
#define SIGSEGV_TRIGGER 0xa0000000
inline ADDR current_pc(ADDR a) { return a & INSTR_ADDR_MASK; }
inline ADDR next_pc(ADDR a) { return (a & INSTR_ADDR_MASK) + INSTR_LEN; }

typedef struct BreakInfo
{
    MIPSInst inst_buf;
    MIPSInst *inst_addr;
} BreakInfo;

/* global variables */
int page_size;
ADDR page_mask;
static BreakInfo bi = {0};
csh cshandle;

static inline int hit_a_break_MIPSInst(MIPSInst *cp)
{
    if (bi.inst_addr == cp)
        return 1;
    else
        return 0;
}

void __attribute__((constructor)) libbreak_init(void)
{
    page_size = getpagesize();
    page_mask = ~((unsigned long)page_size - 1);
    cs_err ret = cs_open(CS_ARCH_MIPS, CS_MODE_MIPS64, &cshandle);
    assert(ret == CS_ERR_OK);
    ret = cs_option(cshandle, CS_OPT_DETAIL, CS_OPT_ON);
    assert(ret == CS_ERR_OK);
}

void __attribute__((destructor)) libbreak_exit(void) { cs_close(&cshandle); }

/* return the value of @i-th genral reg oprand in @insn */
static inline greg_t greg_value(ucontext_t *ucp, cs_insn *insn, int i)
{
    cs_mips_op operand = insn->detail->mips.operands[i];
    assert(operand.type == MIPS_OP_REG);
    greg_t v = ucp->uc_mcontext.gregs[operand.reg - MIPS_REG_0];
    return v;
}

static inline int64_t imm_value(cs_insn *insn, int i)
{
    cs_mips_op operand = insn->detail->mips.operands[i];
    assert(operand.type == MIPS_OP_IMM);
    return operand.imm;
}

int insert_break_next(ucontext_t *ucp)
{
    assert(bi.inst_addr == (MIPSInst *)0);
    /* MIPS delayed slot */
    /*
     * +---------------+
     * |0branch        +---------------+
     * +---------------+       +------ | -----+
     * |1delayed_slot  |       |4temp  |      |
     * +---------------+       +-------V------+
     * |2next(not jump)|       |3next(jump)   |
     * +---------------+       +--------------+
     */
    ADDR pc = ucp->uc_mcontext.pc; /* pc points at 0branch */
    ADDR npc = pc;                 /* next pc */
    cs_insn *insn;
    size_t count = cs_disasm(cshandle, (uint8_t *)pc, 4, pc, 1, &insn);
    assert(count == 1);
    if (cs_insn_group(cshandle, insn, MIPS_GRP_JUMP))
    {
        // bi.delayed_slot = true;
        switch (insn->id)
        {
            case MIPS_INS_BEQ:
            {
                greg_t rs = greg_value(ucp, insn, 0);
                greg_t rt = greg_value(ucp, insn, 1);
                int64_t target_pc = imm_value(insn, 2);
                if (rs == rt)
                {
                    npc = target_pc; /* npc points at 3next */
                }
                else
                {
                    npc += 8; /* npc points at 2next */
                }
            }
            break;
            case MIPS_INS_BNEZ:
            {
                greg_t rs = greg_value(ucp, insn, 0);
                int64_t target_pc = imm_value(insn, 1);
                if (rs != 0)
                {
                    npc = target_pc;
                }
                else
                {
                    npc += 8;
                }
            }
            break;
            default:
                printf("0x%" PRIx64 ":\t", insn->address);
                for(int i=0; i<insn->size; i++){
                    printf("%hhx", insn->bytes[i]);
                }
                printf("\t%s\t\t%s\n", insn->mnemonic, insn->op_str);
                assert(0);
            break;
        }
    }
    else
    {
        npc += 4;
    }
    cs_free(insn, 1);

    LIBBREAK_VERBOSE_OUTPUT("insert break at %p\n", (MIPSInst *)npc);
    int ret = mprotect((void *)(npc & page_mask), page_size,
                       PROT_READ | PROT_WRITE | PROT_EXEC);
    if (ret == -1)
        perror("mprotect");
    bi.inst_buf = *(MIPSInst *)npc;
    *(MIPSInst *)npc = SIGSEGV_TRIGGER;
    bi.inst_addr = (MIPSInst *)npc;

    return 0;
}

int remove_break(ADDR a)
{
    MIPSInst *cp = (MIPSInst *)current_pc(a);
    assert(hit_a_break_MIPSInst(cp));

    *cp = bi.inst_buf;
    bi.inst_addr = (MIPSInst *)0;

    return 0;
}

int hit_a_break(ADDR a)
{
    MIPSInst *cp = (MIPSInst *)current_pc(a);
    return hit_a_break_MIPSInst(cp);
}
