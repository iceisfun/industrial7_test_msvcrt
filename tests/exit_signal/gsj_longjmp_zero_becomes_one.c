/* TC
 * id:    gsj_longjmp_zero_becomes_one
 * area:  exit_signal
 * funcs: longjmp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.13.2.1p4; MS docs longjmp
 */
#include "tc.h"
#include <setjmp.h>
#include <limits.h>

static jmp_buf jb;
static volatile int v;

static int jump_with(int val)
{
    volatile int again = 0;
    int r = setjmp(jb);
    if (r == 0 && !again) { again = 1; v = val; longjmp(jb, v); }
    return r;
}
TC_CASE(zero_to_one) { CHECK_EQ_I(jump_with(0), 1); }
TC_CASE(one) { CHECK_EQ_I(jump_with(1), 1); }
TC_CASE(extremes) { CHECK_EQ_I(jump_with(INT_MAX), INT_MAX); CHECK_EQ_I(jump_with(INT_MIN), INT_MIN); }

int main(void)
{
    tc_begin();
    TC_RUN(zero_to_one); TC_RUN(one); TC_RUN(extremes);
    return tc_end();
}
