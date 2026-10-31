/* TC
 * id:    m99_scalb_legacy
 * area:  math_c99
 * funcs: _scalb
 * procs: _scalbf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _scalb, _scalbf
 * note:  _scalb(double, long). _scalbf resolved dynamically (import libs inconsistent); skipped if absent.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
typedef float (__cdecl *pscf)(float, long);
static pscf p_sf;
static volatile double v_a, v_zero = 0.0, v_one = 1.0;
TC_CASE(basic) { v_a = 1; CHECK_BITS_D(_scalb(v_a, 10), 1024.0); v_a = 3; CHECK_BITS_D(_scalb(v_a, -1), 1.5); }
TC_CASE(zero) { v_a = 0; CHECK_BITS_D(_scalb(v_a, 5), 0.0); }
TC_CASE(floatv) { volatile float f = 1; if (!p_sf) TC_SKIP("no _scalbf"); CHECK_BITS_F(p_sf(f, 3), 8.0f); }
int main(void) { tc_begin(); p_sf = (pscf)tc_crt_proc("_scalbf"); TC_RUN(basic); TC_RUN(zero); TC_RUN(floatv); return tc_end(); }
