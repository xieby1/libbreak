#include "break.h"

#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* abort */

#define NOT_IMPLEMENTED()                                          \
    do                                                             \
    {                                                              \
        fprintf(stderr, "%s: %s: %d: not implemented\n", __FILE__, \
                __FUNCTION__, __LINE__);                           \
        abort();                                                   \
    } while (0)

ADDR current_pc(ADDR a)
{
    NOT_IMPLEMENTED();
    return (ADDR)0;
}

ADDR next_pc(ADDR a)
{
    NOT_IMPLEMENTED();
    return (ADDR)0;
}

int insert_break(ADDR a)
{
    NOT_IMPLEMENTED();
    return (int)0;
}

int insert_break_next(ucontext_t *ucp)
{
    NOT_IMPLEMENTED();
    return (int)0;
}

int remove_break(ADDR a)
{
    NOT_IMPLEMENTED();
    return (int)0;
}

int hit_a_break(ADDR a)
{
    NOT_IMPLEMENTED();
    return (int)0;
}
