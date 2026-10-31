/* TC
 * id:    pfu_sprintf_s_special_floats_old
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  observed
 * ref:   pre-UCRT Microsoft CRT formatting of non-finite doubles (1.#INF, 1.#QNAN)
 * note:  sprintf_s is resolved at run time because mingw's msvcrt.dll link of the inline form pulls in libmingwex vsnprintf. msvcr120 (VS2013) is probed in pfu_sprintf_s_floats_inf_msvcr120_probe.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define sprintf_s tc_p_sprintf_s
static volatile unsigned long long pinf = 0x7FF0000000000000ULL, ninf = 0xFFF0000000000000ULL, qnan = 0x7FF8000000000000ULL;
static double D(volatile unsigned long long *p) { union { unsigned long long u; double d; } x; x.u = *p; return x.d; }

TC_CASE(inf_f)  { char b[32]; sprintf_s(b, 32, "%f", D(&pinf)); CHECK_STR(b, "1.#INF00"); }
TC_CASE(ninf_f) { char b[32]; sprintf_s(b, 32, "%f", D(&ninf)); CHECK_STR(b, "-1.#INF00"); }
TC_CASE(qnan_f) { char b[32]; sprintf_s(b, 32, "%f", D(&qnan)); CHECK_STR(b, "1.#QNAN0"); }
TC_CASE(inf_g)  { char b[32]; sprintf_s(b, 32, "%g", D(&pinf)); CHECK_STR(b, "1.#INF"); }

int main(void) { tc_begin(); TC_LOAD_PROC(sprintf_s); if (!tc_p_sprintf_s) return 2; TC_RUN(inf_f); TC_RUN(ninf_f); TC_RUN(qnan_f); TC_RUN(inf_g); return tc_end(); }
