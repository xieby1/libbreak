#include "break.h"

#include <signal.h>   /* sigaction */
#include <stdio.h>    /* perror, printf */
#include <stdlib.h>   /* abort */
#include <sys/mman.h> /* mmap, mprotect */
#include <ucontext.h> /* ucontext */
#include <unistd.h>   /* getpagesize */

#define PROTS PROT_READ | PROT_WRITE | PROT_EXEC
#define MAPS MAP_PRIVATE | MAP_ANONYMOUS

/* global variables */
static void *page;
static int size;
static struct sigaction sa = {
    .sa_mask = SA_NODEFER,
    .sa_flags = SA_SIGINFO,
};

static void handler(int sig, siginfo_t *info, void *ucontext)
{
    ucontext_t *uc = ucontext;
    unsigned long pc;
#ifdef __x86_64__
    pc = uc->uc_mcontext.gregs[REG_RIP];
#elif defined __mips__
    pc = uc->uc_mcontext.pc;
#endif
    if (hit_a_break(pc))
    { /* epilogue */
        mprotect(page, size, PROT_NONE);
        remove_break(pc);
        printf("remove ");
    }
    else
    { /* prologue */
        mprotect(page, size, PROTS);
        insert_break_next(ucontext);
        printf("insert next_");
    }
    printf("break at %lx\n", pc);
}

void __attribute__((constructor)) init_break_mips(void)
{
    /* init global variables */
    size = getpagesize();
    page = mmap(0, size, PROT_NONE, MAPS, -1, 0);
    if (page == (void *)-1)
    {
        perror("mmap");
        abort();
    }

    /* install signal handler */
    sa.sa_sigaction = handler;
    int ret = sigaction(SIGSEGV, &sa, NULL);
    if (ret == -1)
    {
        perror("sigaction");
        abort();
    }
}

int main(void)
{
    /* a trivial break */
    *(int *)page = 1337;
    printf("%d\n", *(int *)page); /* Expected output: 1337 */

    /* a break after delayed slot */
#ifdef __mips__
    asm volatile(
        ".set noreorder\n\t"
        "beq $v0, $v0, TAKEN\n\t"
        "sd $zero, 0(%0)\n\t" /* explicit delayed slot, trigger sigsegv */
        ".set reorder\n\t"
        "NOT_TAKEN:\n\t"
        "nop\n\t"
        "j END\n\t"
        "TAKEN:\n\t"
        "nop\n\t"
        "END:\n\t"
        :           /* output */
        : "r"(page) /* input */
    );
#endif
    printf("%lx\n", *(long *)page); /* Expected output: 0 */
    return 0;
}
