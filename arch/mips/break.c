#include "break.h"

#define INSTR_LEN 4 /* in byte */
#define INSTR_ADDR_MASK (ADDR)(~(INSTR_LEN - 1))

ADDR current_pc(ADDR a) { return a & INSTR_ADDR_MASK; }

ADDR next_pc(ADDR a) { return (a & INSTR_ADDR_MASK) + INSTR_LEN; }

void __attribute__ ((constructor)) libbreak_init(void)
{
    
}

void __attribute__ ((destructor)) libbreak_exit(void)
{

}

int insert_break(ADDR a) 
{

    return 0;
}
