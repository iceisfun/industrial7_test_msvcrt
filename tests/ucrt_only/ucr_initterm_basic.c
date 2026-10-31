/* TC
 * id:    ucr_initterm_basic
 * area:  ucrt_only
 * funcs: _initterm
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   CRT startup helper (undocumented by MS, used for .CRT$XI* / $XC* initializer tables)
 */
#include "tc.h"

typedef void (__cdecl *vfn)(void);
extern void __cdecl _initterm(vfn *, vfn *);

static int order[8], norder;
static void __cdecl f1(void) { order[norder++] = 1; }
static void __cdecl f2(void) { order[norder++] = 2; }
static void __cdecl f3(void) { order[norder++] = 3; }

TC_CASE(order_123)
{
    vfn t[3]; t[0] = f1; t[1] = f2; t[2] = f3; norder = 0;
    _initterm(t, t + 3);
    CHECK_EQ_I(norder, 3); CHECK_EQ_I(order[0], 1); CHECK_EQ_I(order[1], 2); CHECK_EQ_I(order[2], 3);
}
TC_CASE(null_skipped)
{
    vfn t[5]; t[0] = NULL; t[1] = f1; t[2] = NULL; t[3] = f2; t[4] = NULL; norder = 0;
    _initterm(t, t + 5);
    CHECK_EQ_I(norder, 2); CHECK_EQ_I(order[0], 1); CHECK_EQ_I(order[1], 2);
}
TC_CASE(empty_range)
{
    vfn t[2]; t[0] = f1; t[1] = f2; norder = 0;
    _initterm(t, t);
    CHECK_EQ_I(norder, 0);
}
TC_CASE(last_exclusive)
{
    vfn t[3]; t[0] = f1; t[1] = f2; t[2] = f3; norder = 0;
    _initterm(t, t + 2);
    CHECK_EQ_I(norder, 2); CHECK_EQ_I(order[0], 1); CHECK_EQ_I(order[1], 2);
}

int main(void) { tc_begin(); TC_RUN(order_123); TC_RUN(null_skipped); TC_RUN(empty_range); TC_RUN(last_exclusive); return tc_end(); }
