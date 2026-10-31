/* TC
 * id:    wcs_wcslwr_basic
 * area:  wstring
 * funcs: _wcslwr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strlwr, _wcslwr, _mbslwr (C locale: only A-Z affected)
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(upper_to_lower) { wchar_t b[] = L"HELLO World"; wchar_t *volatile p = b; _wcslwr(p); CHECK_WSTR(b, L"hello world"); }
TC_CASE(already_lower)  { wchar_t b[] = L"abc"; wchar_t *volatile p = b; _wcslwr(p); CHECK_WSTR(b, L"abc"); }
TC_CASE(digits_punct)   { wchar_t b[] = L"123 !@#[]{}~`_-+="; wchar_t *volatile p = b; _wcslwr(p); CHECK_WSTR(b, L"123 !@#[]{}~`_-+="); }
TC_CASE(same_ptr)       { wchar_t b[] = L"ABC"; wchar_t *volatile p = b; CHECK(_wcslwr(p) == b); }
TC_CASE(empty)          { wchar_t b[] = L""; wchar_t *volatile p = b; CHECK(_wcslwr(p) == b); CHECK_EQ_U(b[0], 0); }
TC_CASE(ffff_surrogates)
{
    wchar_t b[] = { 0xFFFF, 0xD83D, 0xDE00, L'A', 0 }; wchar_t *volatile p = b; _wcslwr(p);
    CHECK_EQ_U(b[0], 0xFFFF); CHECK_EQ_U(b[1], 0xD83D); CHECK_EQ_U(b[2], 0xDE00); CHECK_EQ_U(b[3], L'a'); CHECK_EQ_U(b[4], 0);
}
int main(void) { tc_begin(); TC_RUN(upper_to_lower); TC_RUN(already_lower); TC_RUN(digits_punct); TC_RUN(same_ptr); TC_RUN(empty); TC_RUN(ffff_surrogates); return tc_end(); }
