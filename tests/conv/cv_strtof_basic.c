/* TC
 * id:    cv_strtof_basic
 * area:  conv
 * funcs:
 * procs: strtof
 * crt:   >=120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.1.3; MS docs strtod, strtof, wcstof (strtof since VS2013)
 * note:  strtof resolved via procs because mingw's libmingwex supplies a static copy for msvcr120.
 *        availability on msvcr90/100/110 is probed separately (cv_strtof_legacy_probe)
 */
#include "tc.h"
#include <stdlib.h>
#include <float.h>

typedef float (__cdecl *strtof_t)(const char *, char **);
static strtof_t p_strtof;
static float F(const char *volatile s) { return p_strtof(s, NULL); }

TC_CASE(simple) { CHECK_BITS_F(F("1.5"), 1.5f); CHECK_BITS_F(F("  -2.25"), -2.25f); }
TC_CASE(tenth) { CHECK_BITS_F(F("0.1"), 0.1f); CHECK_EQ_U(tc_fbits(F("0.1")), 0x3DCCCCCDu); }
TC_CASE(flt_max) { CHECK_BITS_F(F("3.4028235e38"), FLT_MAX); }
TC_CASE(flt_min) { CHECK_BITS_F(F("1.17549435e-38"), FLT_MIN); }
TC_CASE(denorm_min) { CHECK_EQ_U(tc_fbits(F("1.4e-45")), 1u); }
TC_CASE(neg_zero) { CHECK_EQ_U(tc_fbits(F("-0.0")), 0x80000000u); CHECK_EQ_U(tc_fbits(F("0")), 0u); }
TC_CASE(endptr)
{
    const char *volatile s = "12.5xyz"; char *e = NULL; float f = p_strtof(s, &e);
    CHECK_BITS_F(f, 12.5f); CHECK_EQ_P(e, s + 4);
    s = "abc"; f = p_strtof(s, &e); CHECK_BITS_F(f, 0.0f); CHECK_EQ_P(e, s);
}

int main(void) { tc_begin(); p_strtof = (strtof_t)tc_crt_proc("strtof"); if (!p_strtof) { tc_write("TC FAIL " TC_TEST_ID "::__strtof not exported\n"); return 1; } TC_RUN(simple); TC_RUN(tenth); TC_RUN(flt_max); TC_RUN(flt_min); TC_RUN(denorm_min); TC_RUN(neg_zero); TC_RUN(endptr); return tc_end(); }
