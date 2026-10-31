/* TC
 * id:    ucr_neg_initterm_e_stops_on_error
 * area:  ucrt_only
 * kind:  negative
 * funcs: _initterm_e
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   CRT startup helper (undocumented by MS): returns first nonzero result and stops
 */
#include "tc.h"

typedef int (__cdecl *efn)(void);
extern int __cdecl _initterm_e(efn *, efn *);

static int calls;
static int __cdecl ok(void)    { calls |= 1; return 0; }
static int __cdecl fail7(void) { calls |= 2; return 7; }
static int __cdecl third(void) { calls |= 4; return 0; }
static int __cdecl neg(void)   { calls |= 2; return -3; }

TC_CASE(second_fails)
{
    efn t[3]; t[0] = ok; t[1] = fail7; t[2] = third; calls = 0;
    CHECK_EQ_I(_initterm_e(t, t + 3), 7);
    CHECK_EQ_I(calls, 3);   /* third (bit 4) not called */
}
TC_CASE(first_fails)
{
    efn t[2]; t[0] = fail7; t[1] = third; calls = 0;
    CHECK_EQ_I(_initterm_e(t, t + 2), 7);
    CHECK_EQ_I(calls, 2);
}
TC_CASE(negative_propagated)
{
    efn t[3]; t[0] = ok; t[1] = neg; t[2] = third; calls = 0;
    CHECK_EQ_I(_initterm_e(t, t + 3), -3);
    CHECK_EQ_I(calls, 3);
}

int main(void) { tc_begin(); TC_RUN(second_fails); TC_RUN(first_fails); TC_RUN(negative_propagated); return tc_end(); }
