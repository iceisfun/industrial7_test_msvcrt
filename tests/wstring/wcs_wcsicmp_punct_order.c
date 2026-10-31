/* TC
 * id:    wcs_wcsicmp_punct_order
 * area:  wstring
 * funcs: _wcsicmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _wcsicmp: converts to lowercase before comparing
 * note:  '_' (0x5F) lies between 'Z' (0x5A) and 'a' (0x61). Folding to LOWER (not upper) makes '_' sort below every letter, and 'Z' (->'z' 0x7A) above '_'.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static int icmp(const wchar_t *a, const wchar_t *b)
{ const wchar_t *volatile pa = a, *volatile pb = b; return _wcsicmp(pa, pb); }
TC_CASE(underscore_lt_a) { CHECK(icmp(L"_", L"A") < 0); CHECK(icmp(L"A", L"_") > 0); CHECK(icmp(L"_", L"a") < 0); }
TC_CASE(bracket_lt_a) { CHECK(icmp(L"[", L"a") < 0); CHECK(icmp(L"[", L"A") < 0); }
TC_CASE(caret_lt_a) { CHECK(icmp(L"^", L"a") < 0); CHECK(icmp(L"^", L"A") < 0); }
TC_CASE(z_gt_underscore) { CHECK(icmp(L"Z", L"_") > 0); CHECK(icmp(L"z", L"_") > 0); CHECK(icmp(L"_", L"Z") < 0); }
int main(void) { tc_begin(); TC_RUN(underscore_lt_a); TC_RUN(bracket_lt_a); TC_RUN(caret_lt_a); TC_RUN(z_gt_underscore); return tc_end(); }
