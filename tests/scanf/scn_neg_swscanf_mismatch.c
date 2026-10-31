/* TC
 * id:    scn_neg_swscanf_mismatch
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.2.2 (fwscanf) return value: EOF on input failure, count on matching failure

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const wchar_t *v;

TC_CASE(alpha_for_int) { int a = 9; int r; v = L"abc"; r = swscanf((const wchar_t *)v, L"%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 9); }
TC_CASE(empty_is_eof) { int a = 9; int r; v = L""; r = swscanf((const wchar_t *)v, L"%d", &a); CHECK_EQ_I(r, -1); CHECK_EQ_I(a, 9); }
TC_CASE(blank_is_eof) { int a = 9; int r; v = L"   "; r = swscanf((const wchar_t *)v, L"%d", &a); CHECK_EQ_I(r, -1); CHECK_EQ_I(a, 9); }
TC_CASE(partial_count) { int a = 0, b = 9; int r; v = L"5 x"; r = swscanf((const wchar_t *)v, L"%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 5); CHECK_EQ_I(b, 9); }
TC_CASE(eof_after_first) { int a = 0, b = 9; int r; v = L"5"; r = swscanf((const wchar_t *)v, L"%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 5); CHECK_EQ_I(b, 9); }
TC_CASE(literal_mismatch) { int a = 9; int r; v = L"x5"; r = swscanf((const wchar_t *)v, L"y%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 9); }
TC_CASE(scanset_no_match) { wchar_t b[4] = { L'Z', L'Z', 0, 0 }; int r; v = L"123"; r = swscanf((const wchar_t *)v, L"%l[a-z]", b); CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], L'Z'); }
TC_CASE(suppressed_not_counted) { int r; v = L"abc"; r = swscanf((const wchar_t *)v, L"%*d"); CHECK_EQ_I(r, 0); }
int main(void) { tc_begin(); TC_RUN(alpha_for_int); TC_RUN(empty_is_eof); TC_RUN(blank_is_eof); TC_RUN(partial_count); TC_RUN(eof_after_first); TC_RUN(literal_mismatch); TC_RUN(scanset_no_match); TC_RUN(suppressed_not_counted); return tc_end(); }
