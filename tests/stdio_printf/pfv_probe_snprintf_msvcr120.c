/* TC
 * id:    pfv_probe_snprintf_msvcr120
 * area:  stdio_printf
 * funcs:
 * procs: snprintf
 * crt:   msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.5; MS docs snprintf
 * note:  PROBE: msvcr120.dll (VS2013) exports a C99-conforming snprintf: returns the would-be length (8), NUL terminates, snprintf(NULL,0,..) counts.
 */
#include "tc.h"
#include <stdio.h>
TC_DECL_PROC(int, snprintf, (char *, size_t, const char *, ...));
static const char *vs = "abcdefgh";
TC_CASE(trunc_ret) { char b[16]; int n; if (!tc_p_snprintf) TC_SKIP("not exported"); n = tc_p_snprintf(b, 5, "%s", vs); CHECK_EQ_I(n, 8); }
TC_CASE(trunc_nul) { char b[16]; int i; if (!tc_p_snprintf) TC_SKIP("not exported"); for (i = 0; i < 16; i++) b[i] = 'X'; tc_p_snprintf(b, 5, "%s", vs); CHECK_STR(b, "abcd"); CHECK(b[5] == 'X'); }
TC_CASE(count_only) { int n; if (!tc_p_snprintf) TC_SKIP("not exported"); n = tc_p_snprintf(NULL, 0, "%s", vs); CHECK_EQ_I(n, 8); }
int main(void) { tc_begin(); TC_LOAD_PROC(snprintf); TC_RUN(trunc_ret); TC_RUN(trunc_nul); TC_RUN(count_only); return tc_end(); }
