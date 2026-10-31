/* TC
 * id:    scn_neg_snscanf_truncated
 * area:  scanf
 * funcs: _snscanf _snwscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snscanf, _snwscanf (count = max number of characters to read; returns EOF at end of input before first conversion)
 */
#include "tc.h"
#include <wchar.h>
#include <stdio.h>

static volatile size_t cnt;

TC_CASE(second_conv_cut) { int a = 0, b = 99, r; cnt = 3; r = _snscanf("123 456", cnt, "%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 123); CHECK_EQ_I(b, 99); }
TC_CASE(second_missing)  { int a = 0, b = 99, r; cnt = 1; r = _snscanf("12", cnt, "%d%d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 99); }
TC_CASE(token_cut)       { char s[8] = ""; int r; cnt = 3; r = _snscanf("abcdef", cnt, "%s", s); CHECK_EQ_I(r, 1); CHECK_STR(s, "abc"); }
TC_CASE(cut_before_conv) { int a = 99, r; cnt = 2; r = _snscanf("  12", cnt, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 99); }
TC_CASE(wide_second_cut) { int a = 0, b = 99, r; cnt = 3; r = _snwscanf(L"123 456", cnt, L"%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 123); CHECK_EQ_I(b, 99); }
TC_CASE(wide_token_cut)  { wchar_t s[8] = L""; int r; cnt = 3; r = _snwscanf(L"abcdef", cnt, L"%ls", s); CHECK_EQ_I(r, 1); CHECK_WSTR(s, L"abc"); }
TC_CASE(wide_cut_before) { int a = 99, r; cnt = 2; r = _snwscanf(L"  12", cnt, L"%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 99); }

int main(void) { tc_begin(); TC_RUN(second_conv_cut); TC_RUN(second_missing); TC_RUN(token_cut); TC_RUN(cut_before_conv); TC_RUN(wide_second_cut); TC_RUN(wide_token_cut); TC_RUN(wide_cut_before); return tc_end(); }
