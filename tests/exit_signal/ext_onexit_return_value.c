/* TC
 * id:    ext_onexit_return_value
 * area:  exit_signal
 * funcs: 
 * procs: _onexit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _onexit: returns a pointer to the function if successful, NULL otherwise
 * note:  no-op handlers; they run harmlessly when the parent exits
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *tc_onx_t)(void);
typedef tc_onx_t (__cdecl *tc_pt__onexit)(tc_onx_t); static tc_pt__onexit tc_p__onexit;

static int __cdecl f1(void) { return 0; }
static int __cdecl f2(void) { return 0; }

TC_CASE(result_equals_fn) { tc_onx_t volatile r = tc_p__onexit(f1); CHECK(r == (tc_onx_t)f1); }
TC_CASE(two_functions) {
    tc_onx_t a = tc_p__onexit(f1), b = tc_p__onexit(f2);
    CHECK(a == (tc_onx_t)f1); CHECK(b == (tc_onx_t)f2); CHECK(a != b);
}
TC_CASE(same_twice) {
    tc_onx_t a = tc_p__onexit(f2), b = tc_p__onexit(f2);
    CHECK(a == (tc_onx_t)f2); CHECK(b == (tc_onx_t)f2);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_onexit); if (!tc_p__onexit) { tc_write("TC FAIL " TC_TEST_ID "::__onexit missing\n"); return 1; } TC_RUN(result_equals_fn); TC_RUN(two_functions); TC_RUN(same_twice); return tc_end(); }
