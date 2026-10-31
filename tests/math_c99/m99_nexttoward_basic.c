/* TC
 * id:    m99_nexttoward_basic
 * area:  math_c99
 * funcs: 
 * procs: nexttoward nexttowardf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs nextafter, nextafterf, nexttoward, nexttowardf
 * note:  mingw ucrtbase import lib lacks nexttoward, so resolved at run time. long double is 64-bit in the MS ABI, but mingw long double is 80-bit, so the procs are typed as taking double.
 */
#include "tc.h"
#include <float.h>

typedef double (__cdecl *nt_t)(double, double);
typedef float  (__cdecl *ntf_t)(float, double);
static nt_t p_nt; static ntf_t p_ntf;

TC_CASE(exported) { CHECK(p_nt != NULL); CHECK(p_ntf != NULL); }
TC_CASE(up) { volatile double a = 1.0, b = 2.0; if (!p_nt) TC_SKIP("missing"); CHECK_BITS_D(p_nt(a, b), 1.0 + DBL_EPSILON); }
TC_CASE(equal) { volatile double a = 1.0, b = 1.0; if (!p_nt) TC_SKIP("missing"); CHECK_BITS_D(p_nt(a, b), 1.0); }
TC_CASE(float_up) { volatile float a = 1.0f; volatile double b = 2.0; if (!p_ntf) TC_SKIP("missing"); CHECK_BITS_F(p_ntf(a, b), 1.0f + FLT_EPSILON); }
TC_CASE(zero_down) { volatile double a = 0.0, b = -1.0; if (!p_nt) TC_SKIP("missing"); CHECK_EQ_U(tc_dbits(p_nt(a, b)), 0x8000000000000001ULL); }

int main(void)
{
    tc_begin();
    p_nt = (nt_t)tc_crt_proc("nexttoward"); p_ntf = (ntf_t)tc_crt_proc("nexttowardf");
    TC_RUN(exported); TC_RUN(up); TC_RUN(equal); TC_RUN(float_up); TC_RUN(zero_down);
    return tc_end();
}
