/* TC
 * id:    gsj_longjmp_setjmp_basic
 * area:  exit_signal
 * funcs: longjmp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.13.1.1, 7.13.2.1; MS docs setjmp, longjmp
 * note:  setjmp is a compiler/mingw static symbol on ucrtbase (not in its import table); only longjmp is import-checked.
 */
#include "tc.h"
#include <setjmp.h>

static jmp_buf jb;
static volatile int v_val = 5;

TC_CASE(first_returns_zero)
{
    volatile int r = setjmp(jb);
    CHECK_EQ_I(r, 0);
}
TC_CASE(value_returned)
{
    volatile int n = 0;
    int r = setjmp(jb);
    if (r == 0) { n = 1; longjmp(jb, v_val); }
    CHECK_EQ_I(r, 5);
    CHECK_EQ_I(n, 1);
}
TC_CASE(negative_value)
{
    volatile int neg = -3;
    int r = setjmp(jb);
    if (r == 0) longjmp(jb, neg);
    CHECK_EQ_I(r, -3);
}
TC_CASE(volatile_locals_latest)
{
    volatile int a = 1; volatile long long b = 10;
    int r = setjmp(jb);
    if (r == 0) { a = 2; b = 20; longjmp(jb, 9); }
    CHECK_EQ_I(r, 9); CHECK_EQ_I(a, 2); CHECK_EQ_I(b, 20);
}
TC_CASE(loop_same_site)
{
    volatile int hits = 0, last = 0; volatile int i = 0;
    int r = setjmp(jb);
    hits++;
    last = r;
    if (i < 3) { i++; longjmp(jb, i * 10); }
    CHECK_EQ_I(hits, 4);
    CHECK_EQ_I(last, 30);
}

int main(void)
{
    tc_begin();
    TC_RUN(first_returns_zero); TC_RUN(value_returned); TC_RUN(negative_value);
    TC_RUN(volatile_locals_latest); TC_RUN(loop_same_site);
    return tc_end();
}
