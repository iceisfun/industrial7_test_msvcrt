/* TC
 * id:    gsj_longjmp_across_frames
 * area:  exit_signal
 * funcs: longjmp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.13.2.1; MS docs longjmp
 */
#include "tc.h"
#include <setjmp.h>
#include <malloc.h>

static jmp_buf jb, jb2;
static volatile int v_one = 1, depth_reached;

__attribute__((noinline)) static void l3(int v) { longjmp(jb, v); }
__attribute__((noinline)) static void l2(int v) { volatile int x = v; l3(x); depth_reached = 99; }
__attribute__((noinline)) static void l1(int v) { volatile int x = v; l2(x); depth_reached = 99; }
__attribute__((noinline)) static int rec(int n)
{
    volatile char pad[64]; pad[0] = (char)n;
    if (n == 0) longjmp(jb, 42);
    return rec(n - 1) + pad[0];
}
__attribute__((noinline)) static void with_alloca(int n)
{
    volatile char *p = (volatile char *)alloca(n + v_one);
    p[0] = 1;
    longjmp(jb, 8);
}
__attribute__((noinline)) static int big_locals(void)
{
    volatile char buf[4096]; int i, s = 0;
    for (i = 0; i < 4096; i += 64) buf[i] = 1;
    for (i = 0; i < 4096; i += 64) s += buf[i];
    return s;
}

TC_CASE(three_nested)
{
    int r = setjmp(jb);
    if (r == 0) l1(11);
    CHECK_EQ_I(r, 11); CHECK_EQ_I(depth_reached, 0);
}
TC_CASE(deep_recursion)
{
    int r = setjmp(jb);
    if (r == 0) rec(200);
    CHECK_EQ_I(r, 42);
    CHECK_EQ_I(big_locals(), 64);
}
TC_CASE(nested_buffers)
{
    volatile int stage = 0;
    int r2, r1 = setjmp(jb);
    if (r1 == 0) {
        r2 = setjmp(jb2);
        if (r2 == 0) { stage = 1; longjmp(jb2, 3); }
        CHECK_EQ_I(r2, 3); CHECK_EQ_I(stage, 1);
        stage = 2; longjmp(jb, 4);
    }
    CHECK_EQ_I(r1, 4); CHECK_EQ_I(stage, 2);
}
TC_CASE(same_function)
{
    int r = setjmp(jb);
    if (r == 0) longjmp(jb, 6);
    CHECK_EQ_I(r, 6);
}
TC_CASE(alloca_skipped)
{
    int r = setjmp(jb);
    if (r == 0) with_alloca(300);
    CHECK_EQ_I(r, 8);
    CHECK_EQ_I(big_locals(), 64);
}

int main(void)
{
    tc_begin();
    TC_RUN(three_nested); TC_RUN(deep_recursion); TC_RUN(nested_buffers); TC_RUN(same_function); TC_RUN(alloca_skipped);
    return tc_end();
}
