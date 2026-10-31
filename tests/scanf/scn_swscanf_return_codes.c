/* TC
 * id:    scn_swscanf_return_codes
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.2 (EOF if input failure before first conversion, else count); MS docs swscanf return value
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(empty) { int a = 5, r = swscanf(L"", L"%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 5); }
TC_CASE(only_ws) { int a = 5, r = swscanf(L"  ", L"%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 5); }
TC_CASE(mismatch) { int a = 5, r = swscanf(L"x", L"%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 5); }
TC_CASE(partial) { int a = 0, b = 5, c = 6, r = swscanf(L"1 2 x", L"%d %d %d", &a, &b, &c); CHECK_EQ_I(r, 2); CHECK_EQ_I(b, 2); CHECK_EQ_I(c, 6); }
TC_CASE(partial_eof) { int a = 0, b = 5, r = swscanf(L"7", L"%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(b, 5); }
TC_CASE(literal_only_mismatch) { int r = swscanf(L"abc", L"xyz"); CHECK_EQ_I(r, 0); }

int main(void) { tc_begin(); TC_RUN(empty); TC_RUN(only_ws); TC_RUN(mismatch); TC_RUN(partial); TC_RUN(partial_eof); TC_RUN(literal_only_mismatch); return tc_end(); }
