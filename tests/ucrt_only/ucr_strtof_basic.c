/* TC
 * id:    ucr_strtof_basic
 * area:  ucrt_only
 * funcs:
 * procs: strtof
 * crt:   win10 msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C99 7.20.1.3 strtof; MS docs strtof (VS2013+)
 * note:  "inf" parsing asserted on ucrtbase only (msvcr120 support for INF/NAN spellings is not documented).
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_s;
typedef float (__cdecl *strtof_t)(const char *, char **);
static strtof_t p_strtof;
#define strtof p_strtof

TC_CASE(one_point_five) { char *e; float f; v_s = "1.5"; f = strtof(v_s, &e); CHECK_EQ_U(tc_fbits(f), 0x3FC00000); CHECK_EQ_P(e, v_s + 3); }
TC_CASE(flt_max) { char *e; float f; v_s = "3.4028235e38"; f = strtof(v_s, &e); CHECK_EQ_U(tc_fbits(f), 0x7F7FFFFF); CHECK_EQ_P(e, v_s + 12); }
TC_CASE(min_denormal) { char *e; float f; v_s = "1e-45"; f = strtof(v_s, &e); CHECK_EQ_U(tc_fbits(f), 1); CHECK_EQ_P(e, v_s + 5); }
#ifdef TC_UCRT
TC_CASE(inf) { char *e; float f; v_s = "inf"; f = strtof(v_s, &e); CHECK_EQ_U(tc_fbits(f), 0x7F800000); CHECK_EQ_P(e, v_s + 3); }
#endif
TC_CASE(neg_zero) { char *e; float f; v_s = "-0"; f = strtof(v_s, &e); CHECK_EQ_U(tc_fbits(f), 0x80000000u); CHECK_EQ_P(e, v_s + 2); }

int main(void) { tc_begin(); p_strtof = (strtof_t)tc_crt_proc("strtof"); if (!p_strtof) { tc_write("TC FAIL strtof missing\n"); return 1; } TC_RUN(one_point_five); TC_RUN(flt_max); TC_RUN(min_denormal); 
#ifdef TC_UCRT
    TC_RUN(inf);
#endif
    TC_RUN(neg_zero); return tc_end(); }
