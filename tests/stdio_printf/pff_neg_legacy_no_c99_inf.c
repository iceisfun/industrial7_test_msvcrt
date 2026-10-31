/* TC
 * id:    pff_neg_legacy_no_c99_inf
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   win7
 * tier:  p1
 * conf:  observed
 * ref:   pre-UCRT CRTs print 1.#INF00 / 1.#QNAN00 (see MS UCRT breaking-changes page for the change)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(f_inf)  { char b[64]; volatile double d = mk(0x7FF0000000000000ULL); sprintf(b, "%f", d);
    CHECK(strchr(b, '#') != NULL); CHECK(strstr(b, "inf") == NULL); CHECK(strstr(b, "INF") != NULL); }
TC_CASE(f_qnan) { char b[64]; volatile double d = mk(0x7FF8000000000000ULL); sprintf(b, "%f", d);
    CHECK(strcmp(b, "nan") != 0); CHECK(strchr(b, '#') != NULL); }
TC_CASE(g_inf)  { char b[64]; volatile double d = mk(0x7FF0000000000000ULL); sprintf(b, "%g", d);
    CHECK(strchr(b, '#') != NULL); CHECK(strstr(b, "inf") == NULL); }

int main(void) { tc_begin(); TC_RUN(f_inf); TC_RUN(f_qnan); TC_RUN(g_inf); return tc_end(); }
