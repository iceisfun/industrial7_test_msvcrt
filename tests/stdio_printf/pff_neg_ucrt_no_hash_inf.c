/* TC
 * id:    pff_neg_ucrt_no_hash_inf
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Universal CRT breaking changes" (printf of infinity/NaN no longer 1.#INF / 1.#QNAN; now inf / nan)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static void chk(const char *fmt, double v)
{
    char b[64]; volatile double d = v;
    sprintf(b, fmt, d);
    CHECK(strchr(b, '#') == NULL);
    CHECK(strncmp(b, "1.", 2) != 0);
    CHECK(strncmp(b, "-1.", 3) != 0);
}
TC_CASE(inf)  { chk("%f", mk(0x7FF0000000000000ULL)); chk("%f", mk(0xFFF0000000000000ULL)); }
TC_CASE(qnan) { chk("%f", mk(0x7FF8000000000000ULL)); chk("%f", mk(0xFFF8000000000000ULL)); }
TC_CASE(snan) { chk("%f", mk(0x7FF0000000000001ULL)); }
TC_CASE(e_variants) { chk("%e", mk(0x7FF0000000000000ULL)); chk("%e", mk(0x7FF8000000000000ULL)); chk("%.3E", mk(0xFFF8000000000000ULL)); }
TC_CASE(g_variants) { chk("%g", mk(0x7FF0000000000000ULL)); chk("%G", mk(0x7FF8000000000000ULL)); chk("%10.2g", mk(0xFFF0000000000000ULL)); }

int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(qnan); TC_RUN(snan); TC_RUN(e_variants); TC_RUN(g_variants); return tc_end(); }
