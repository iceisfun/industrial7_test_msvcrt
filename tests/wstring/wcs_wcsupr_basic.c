/* TC
 * id:    wcs_wcsupr_basic
 * area:  wstring
 * funcs: _wcsupr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strupr, _wcsupr, _mbsupr (C locale: only a-z affected)
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(lower_to_upper) { wchar_t b[] = L"hello World"; wchar_t *volatile p = b; _wcsupr(p); CHECK_WSTR(b, L"HELLO WORLD"); }
TC_CASE(already_upper)  { wchar_t b[] = L"ABC"; wchar_t *volatile p = b; _wcsupr(p); CHECK_WSTR(b, L"ABC"); }
TC_CASE(digits_punct)   { wchar_t b[] = L"123 !@#[]{}~`_-+="; wchar_t *volatile p = b; _wcsupr(p); CHECK_WSTR(b, L"123 !@#[]{}~`_-+="); }
TC_CASE(same_ptr)       { wchar_t b[] = L"abc"; wchar_t *volatile p = b; CHECK(_wcsupr(p) == b); }
TC_CASE(empty)          { wchar_t b[] = L""; wchar_t *volatile p = b; CHECK(_wcsupr(p) == b); CHECK_EQ_U(b[0], 0); }
TC_CASE(ffff_surrogates)
{
    wchar_t b[] = { 0xFFFF, 0xD83D, 0xDE00, L'a', 0 }; wchar_t *volatile p = b; _wcsupr(p);
    CHECK_EQ_U(b[0], 0xFFFF); CHECK_EQ_U(b[1], 0xD83D); CHECK_EQ_U(b[2], 0xDE00); CHECK_EQ_U(b[3], L'A'); CHECK_EQ_U(b[4], 0);
}
int main(void) { tc_begin(); TC_RUN(lower_to_upper); TC_RUN(already_upper); TC_RUN(digits_punct); TC_RUN(same_ptr); TC_RUN(empty); TC_RUN(ffff_surrogates); return tc_end(); }
