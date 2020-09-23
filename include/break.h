#ifndef LIBBREAK_BREAK_H
#define LIBBREAK_BREAK_H
#define _GNU_SOURCE
#include <ucontext.h>
typedef unsigned long ADDR;

/*!
 * @param[in] ADDR may points at the start of instr or the middle
 * @param[out] ADDR points at the start of the instr
 */
ADDR current_pc(ADDR);

/*!
 * @param[in] ADDR points at the start of instr or the middle
 * @param[out] ADDR points at the next start of the instr
 */
ADDR next_pc(ADDR);

/*
 * @param[in] insert a break instruction at ADDR
 * @param[out] 0 success, others fail
 */
int insert_break(ADDR);

/*
 * @param[in] insert a break instruction at next inst of current context,
 * ucontext can be retrived by many means, refering to `man getcontext`
 * @param[out] 0 success, others fail
 * insert_break_next is quite different to insert_break, due to next
 * instruction(s) heavily depends on context. E.g. next of a conditional
 * jump instruction has two directions, [jump] or [not jump]. The next of a
 * [not jump] is trivial, while the next of [jump] depends on the condition
 * register/memory/... which only available during executing on this
 * conditional jump instruction.
 */
int insert_break_next(ucontext_t *);

/*
 * @param[in] remove the break instruction at ADDR
 * @param[out] 0 success, other fail
 */
int remove_break(ADDR);

/*
 * @param[in] check whether ADDR is a break trigger
 * @param[out] none-zero: yes it is a break, zero: no it is not a break
 */
int hit_a_break(ADDR);
#endif
