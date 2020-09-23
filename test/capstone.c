// Expected output:
// 0x1359:	push		rbp
// 0x135a:	mov		rax, qword ptr [rip + 0x13b8]
#include "capstone/capstone.h"

#include <assert.h> /* assert */
#include <stdio.h>  /* printf */

/* global variables */
csh cshandle;
size_t count;
#define CODE "\x55\x48\x8b\x05\xb8\x13\x00\x00"
cs_insn *insn;

static void __attribute__((constructor)) init_capstone_test(void)
{
    cs_err ret = cs_open(CS_ARCH_X86, CS_MODE_64, &cshandle);
    assert(ret == CS_ERR_OK);
}

static void __attribute__((destructor)) exit_capstone_test(void)
{
    cs_free(insn, count);
    cs_close(&cshandle);
}

int main(void)
{
    count = cs_disasm(cshandle, (unsigned char *)CODE, sizeof(CODE) - 1, 0x1359,
                      0, &insn);
    assert(count > 0);
    for (int i = 0; i < count; i++)
    {
        printf("0x%" PRIx64 ":\t%s\t\t%s\n", insn[i].address, insn[i].mnemonic,
               insn[i].op_str);
    }
    return 0;
}
