/* TC
 * id:    pfi_probe_snprintf_zero_count
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf
 * note:  PROBE: legacy _snprintf with count 0 returns -1 (output would not fit); NULL buffer with count 0 (iph behaviour not asserted); result is either -1 (legacy overflow report) or 1 (required length) - which one differs by CRT and is unknown to the author.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile f = "%d";
TC_CASE(count0) { char b[8]; int n; tc_iph_install(); tc_iph_count = 0; b[0] = 'Z'; b[1] = 0; n = _snprintf(b, 0, f, 5); if (TC_AT_LEAST(140)) CHECK(n == -1 || n == 1); else CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 'Z'); CHECK(tc_iph_count == 0); }
TC_CASE(null0) { int n; char *volatile p = NULL; tc_iph_install(); n = _snprintf(p, 0, f, 5); CHECK(n == -1 || n == 1); }
int main(void) { tc_begin(); TC_RUN(count0); TC_RUN(null0); return tc_end(); }
