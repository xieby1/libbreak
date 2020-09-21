#include "break.h"

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
    /* for branch target */
    MIPSInst inst_buf2;
    MIPSInst *inst_addr2;
} BreakInfo;

int page_size;
ADDR page_mask;
static BreakInfo bi = {0};

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
}

int insert_break_next(ADDR a)
{
    MIPSInst *np = (MIPSInst *)next_pc(a);
    int ret = mprotect((void *)((ADDR)np & page_mask), page_size,
                       PROT_READ | PROT_WRITE | PROT_EXEC);
    if (ret == -1)
        perror("mprotect");
    bi.inst_buf = *np;
    *np = SIGSEGV_TRIGGER;
    bi.inst_addr = np;

    return 0;
}

int remove_break(ADDR a)
{
    MIPSInst *cp = (MIPSInst *)current_pc(a);
    if (!hit_a_break_MIPSInst(cp))
        return -1;

    *cp = bi.inst_buf;

    return 0;
}

int hit_a_break(ADDR a)
{
    MIPSInst *cp = (MIPSInst *)current_pc(a);
    return hit_a_break_MIPSInst(cp);
}
