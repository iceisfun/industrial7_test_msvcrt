/* TC
 * id:    wcs_wcsncat_basic
 * area:  wstring
 * funcs: wcsncat
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.3.2; MS docs strncat, wcsncat
 * note:  
 */
#include "tc.h"
#include <wchar.h>

static const wchar_t *volatile ps = L"abcdef";

TC_CASE(n_less) { wchar_t d[16] = L"xy"; int i; for (i = 3; i < 16; i++) d[i] = 0x5555; wcsncat(d, ps, 3); CHECK_WSTR(d, L"xyabc"); CHECK_EQ_I(d[5], 0); CHECK_EQ_I(d[6], 0x5555); }
TC_CASE(n_greater) { wchar_t d[16] = L"xy"; wcsncat(d, ps, 10); CHECK_WSTR(d, L"xyabcdef"); }
TC_CASE(n_zero) { wchar_t d[16] = L"xy"; d[3] = 0x5555; wcsncat(d, ps, 0); CHECK_WSTR(d, L"xy"); CHECK_EQ_I(d[2], 0); CHECK_EQ_I(d[3], 0x5555); }
TC_CASE(dest_empty) { wchar_t d[16] = { 0 }; CHECK_EQ_P(wcsncat(d, ps, 2), d); CHECK_WSTR(d, L"ab"); }
TC_CASE(n_one) { wchar_t d[16] = L"q"; wcsncat(d, ps, 1); CHECK_WSTR(d, L"qa"); CHECK_EQ_I(d[2], 0); }
TC_CASE(no_padding) { wchar_t d[16]; int i; static const wchar_t s[] = L"z"; const wchar_t *volatile p = s;
    for (i = 0; i < 16; i++) d[i] = 0x5555; d[0] = 'a'; d[1] = 0;
    wcsncat(d, p, 10);
    CHECK_WSTR(d, L"az"); CHECK_EQ_I(d[3], 0x5555); CHECK_EQ_I(d[11], 0x5555); }
int main(void) { tc_begin(); TC_RUN(n_less); TC_RUN(n_greater); TC_RUN(n_zero); TC_RUN(dest_empty); TC_RUN(n_one); TC_RUN(no_padding); return tc_end(); }
