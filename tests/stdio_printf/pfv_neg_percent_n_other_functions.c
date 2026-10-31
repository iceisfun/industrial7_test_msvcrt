/* TC
 * id:    pfv_neg_percent_n_other_functions
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _scprintf _snwprintf fprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf / _set_printf_count_output: %n disabled by default -> invalid parameter handler
 * note:  swprintf dropped from funcs: its signature differs between generations; _snwprintf covers the wide path.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static volatile int tgt;
TC_CASE(snprintf_n) { char b[16]; int n; tgt = 0x5a5a; tc_iph_count = 0; n = _snprintf(b, sizeof b, "ab%n", (int *)&tgt); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(tgt, 0x5a5a); }
TC_CASE(scprintf_n) { int n; tgt = 0x5a5a; tc_iph_count = 0; n = _scprintf("ab%n", (int *)&tgt); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(tgt, 0x5a5a); }
TC_CASE(snwprintf_n) { wchar_t b[16]; int n; tgt = 0x5a5a; tc_iph_count = 0; n = _snwprintf(b, 16, L"ab%n", (int *)&tgt); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(tgt, 0x5a5a); }
TC_CASE(fprintf_n) { FILE *f = fopen("tc_pfv_pn.tmp", "w"); int n; CHECK(f != NULL); if (!f) return; tgt = 0x5a5a; tc_iph_count = 0; n = fprintf(f, "ab%n", (int *)&tgt); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(tgt, 0x5a5a); fclose(f); remove("tc_pfv_pn.tmp"); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(snprintf_n); TC_RUN(scprintf_n); TC_RUN(snwprintf_n); TC_RUN(fprintf_n); return tc_end(); }
