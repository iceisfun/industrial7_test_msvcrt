/* TC
 * id:    ucr_initterm_e_basic
 * area:  ucrt_only
 * funcs: _initterm_e
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   CRT startup helper (undocumented by MS)
 */
#include "tc.h"

typedef int (__cdecl *efn)(void);
extern int __cdecl _initterm_e(efn *, efn *);

static int calls;
static int __cdecl ok1(void) { calls++; return 0; }
static int __cdecl ok2(void) { calls += 10; return 0; }

TC_CASE(all_ok)
{
    efn t[2]; t[0] = ok1; t[1] = ok2; calls = 0;
    CHECK_EQ_I(_initterm_e(t, t + 2), 0);
    CHECK_EQ_I(calls, 11);
}
TC_CASE(null_skipped)
{
    efn t[3]; t[0] = NULL; t[1] = ok1; t[2] = NULL; calls = 0;
    CHECK_EQ_I(_initterm_e(t, t + 3), 0);
    CHECK_EQ_I(calls, 1);
}
TC_CASE(empty_range)
{
    efn t[1]; t[0] = ok1; calls = 0;
    CHECK_EQ_I(_initterm_e(t, t), 0);
    CHECK_EQ_I(calls, 0);
}

int main(void) { tc_begin(); TC_RUN(all_ok); TC_RUN(null_skipped); TC_RUN(empty_range); return tc_end(); }
