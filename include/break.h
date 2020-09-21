#ifndef LIBBREAK_BREAK_H
#define LIBBREAK_BREAK_H
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
 * @param[in] insert a break instruction at next inst of ADDR
 * @param[out] 0 success, others fail
 */
int insert_break_next(ADDR);

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
