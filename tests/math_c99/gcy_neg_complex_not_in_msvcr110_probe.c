/* TC
 * id:    gcy_neg_complex_not_in_msvcr110_probe
 * area:  math_c99
 * funcs:
 * procs: _errno
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs C complex math (cacos etc.): Requirements list VS2013+ (msvcr120) as first
 * note:  PROBING: GetProcAddress(CRT, name) == NULL for the C99 complex functions in every CRT older than msvcr120.
 */
#include "tc.h"
static const char *const g1[] = { "cacos", "casin", "catan", "ccos", "csin", "ctan", 0 };
static const char *const g2[] = { "cacosh", "casinh", "catanh", "ccosh", "csinh", "ctanh", 0 };
static const char *const g3[] = { "cexp", "clog", 0 };
static const char *const g4[] = { "cexpf", "clogf", 0 };
static void absent(const char *const *g, const char *grp)
{
    int i;
    for (i = 0; g[i]; i++) if (tc_crt_proc(g[i])) { tc_fail_expr(__FILE__, __LINE__, g[i]); (void)grp; }
}
TC_CASE(control_present) { CHECK(tc_crt_proc("_errno") != NULL); }
TC_CASE(trig) { absent(g1, "trig"); }
TC_CASE(hyper) { absent(g2, "hyper"); }
TC_CASE(expl) { absent(g3, "expl"); }
TC_CASE(floatv) { absent(g4, "floatv"); }
int main(void) { tc_begin(); TC_RUN(control_present); TC_RUN(trig); TC_RUN(hyper); TC_RUN(expl); TC_RUN(floatv); return tc_end(); }
