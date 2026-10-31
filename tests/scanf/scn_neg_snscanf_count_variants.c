/* TC
 * id:    scn_neg_snscanf_count_variants
 * area:  scanf
 * kind:  negative
 * funcs: _snscanf _snwscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snscanf, _snwscanf (count = maximum number of characters to read; wide count is in wide characters); C11 7.21.6.2p3
 * note:  Count boundaries that must make a conversion fail. When input is exhausted mid-conversion the exact return (0 vs EOF) is not asserted, only that nothing was stored and no success is reported.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#ifdef _UCRT
#include <stdarg.h>
static int tcx_snwscanf(const wchar_t *s, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswscanf(_CRT_INTERNAL_LOCAL_SCANF_OPTIONS, s, n, f, NULL, ap); va_end(ap); return r; }
#undef _snwscanf
#define _snwscanf tcx_snwscanf
#endif

static volatile size_t cnt;

TC_CASE(cut_between_sign_digit) { int a = 99, r; cnt = 1; r = _snscanf("-5", cnt, "%d", &a); CHECK(r <= 0); CHECK_EQ_I(a, 99); }
TC_CASE(cut_mid_literal) { int a = 99, r; cnt = 2; r = _snscanf("abc7", cnt, "abc%d", &a); CHECK(r <= 0); CHECK_EQ_I(a, 99); }
TC_CASE(cut_after_ws_before_conv_eof) { int a = 99, r; cnt = 3; r = _snscanf("   12", cnt, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 99); }
TC_CASE(wide_count_in_chars) { int a = 99, r; cnt = 2; r = _snwscanf(L"1234", cnt, L"%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); }
TC_CASE(wide_cut_between_sign_digit) { int a = 99, r; cnt = 1; r = _snwscanf(L"-5", cnt, L"%d", &a); CHECK(r <= 0); CHECK_EQ_I(a, 99); }
TC_CASE(count_1_second_int_missing) { int a = 0, b = 99, r; cnt = 1; r = _snscanf("12", cnt, "%d%d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 99); }

int main(void) { tc_begin(); TC_RUN(cut_between_sign_digit); TC_RUN(cut_mid_literal); TC_RUN(cut_after_ws_before_conv_eof); TC_RUN(wide_count_in_chars);
    TC_RUN(wide_cut_between_sign_digit); TC_RUN(count_1_second_int_missing); return tc_end(); }
