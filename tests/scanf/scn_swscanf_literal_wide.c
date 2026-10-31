/* TC
 * id:    scn_swscanf_literal_wide
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.2 (fwscanf directives); MS docs scanf width/type specifications
 * note:  wide-character literal comparison is locale independent
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(literal_e_acute) { int v = 0, r; r = swscanf(L"é=5", L"é=%d", &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 5); }
TC_CASE(literal_mismatch_acute) { int v = 9, r; r = swscanf(L"e=5", L"é=%d", &v); CHECK_EQ_I(r, 0); CHECK_EQ_I(v, 9); }
TC_CASE(scanset_negated) { wchar_t b[16]; int r, n = -1; b[0] = 0; r = swscanf(L"abcédef", L"%l[^é]%n", b, &n); CHECK_EQ_I(r, 1); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(n, 3); }
TC_CASE(lc_cjk) { wchar_t c = 0; int r = swscanf(L"中", L"%lc", &c); CHECK_EQ_I(r, 1); CHECK_EQ_U(c, 0x4e2d); }
TC_CASE(ls_mixed) { wchar_t b[16]; int r; b[0] = 0; r = swscanf(L"ab中c", L"%ls", b); CHECK_EQ_I(r, 1); CHECK_WSTR(b, L"ab中" L"c"); }
TC_CASE(case_sensitive) { int r = swscanf(L"X", L"x"); CHECK_EQ_I(r, 0); }

int main(void) { tc_begin(); TC_RUN(literal_e_acute); TC_RUN(literal_mismatch_acute); TC_RUN(scanset_negated); TC_RUN(lc_cjk); TC_RUN(ls_mixed); TC_RUN(case_sensitive); return tc_end(); }
