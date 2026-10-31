/* TC
 * id:    pfi_probe_hh_jzt_old
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf size prefixes (hh/j/z/t are C99, not in VS2010 and earlier)
 * note:  PROBE: what old CRTs do with unsupported length modifiers hh j z t (treated as unknown -> iph/garbage). Assertions: the call returns (no crash) and, if the invalid-parameter handler ran, the return value is -1. No output text is asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile fz = "%zu"; static char *volatile fh = "%hhd"; static char *volatile fj = "%jd"; static char *volatile ft = "%td";
TC_CASE(z) { char b[64]; int n; tc_iph_count = 0; b[0] = 0; n = sprintf(b, fz, (size_t)7); if (tc_iph_count > 0) CHECK_EQ_I(n, -1); else CHECK(n >= 0 || n == -1); }
TC_CASE(hh) { char b[64]; int n; tc_iph_count = 0; b[0] = 0; n = sprintf(b, fh, 7); if (tc_iph_count > 0) CHECK_EQ_I(n, -1); else CHECK(n >= 0 || n == -1); }
TC_CASE(j) { char b[64]; int n; tc_iph_count = 0; b[0] = 0; n = sprintf(b, fj, (intmax_t)7); if (tc_iph_count > 0) CHECK_EQ_I(n, -1); else CHECK(n >= 0 || n == -1); }
TC_CASE(t) { char b[64]; int n; tc_iph_count = 0; b[0] = 0; n = sprintf(b, ft, (ptrdiff_t)7); if (tc_iph_count > 0) CHECK_EQ_I(n, -1); else CHECK(n >= 0 || n == -1); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(z); TC_RUN(hh); TC_RUN(j); TC_RUN(t); return tc_end(); }
