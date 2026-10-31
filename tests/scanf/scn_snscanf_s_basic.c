/* TC
 * id:    scn_snscanf_s_basic
 * area:  scanf
 * funcs: _snscanf_s _snwscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snscanf_s, _snscanf_s_l, _snwscanf_s, _snwscanf_s_l (argument order: buffer, max count, format, then args with sizes)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(count_limits) { char a[16]; int n; static volatile const char *v; v = "abc def"; n = _snscanf_s((const char *)v, 3, "%s", a, (unsigned)16); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); }
TC_CASE(count_cuts_token) { char a[16]; int n; static volatile const char *v; v = "abcdef"; n = _snscanf_s((const char *)v, 4, "%s", a, (unsigned)16); CHECK_EQ_I(n, 1); CHECK_STR(a, "abcd"); }
TC_CASE(unterminated_src) { char src[4]; char a[8]; int n, i; for (i = 0; i < 4; i++) src[i] = (char)('a' + i); for (i = 0; i < 8; i++) a[i] = 'Z'; n = _snscanf_s(src, 4, "%s", a, (unsigned)8); CHECK_EQ_I(n, 1); CHECK_STR(a, "abcd"); }
TC_CASE(int_and_s) { char a[8]; int n, x = 0; static volatile const char *v; v = "17 xy tail"; n = _snscanf_s((const char *)v, 5, "%d %s", &x, a, (unsigned)8); CHECK_EQ_I(n, 2); CHECK_EQ_I(x, 17); CHECK_STR(a, "xy"); }
TC_CASE(wide_count_size) { wchar_t a[16]; int n; static volatile const wchar_t *v; v = L"abc def"; n = _snwscanf_s((const wchar_t *)v, 3, L"%s", a, (unsigned)16); CHECK_EQ_I(n, 1); CHECK_WSTR(a, L"abc"); }
TC_CASE(wide_unterminated) { wchar_t src[3]; wchar_t a[4]; int n; src[0] = L'x'; src[1] = L'y'; src[2] = L'z'; n = _snwscanf_s(src, 3, L"%s", a, (unsigned)4); CHECK_EQ_I(n, 1); CHECK_WSTR(a, L"xyz"); }
int main(void) { tc_begin(); TC_RUN(count_limits); TC_RUN(count_cuts_token); TC_RUN(unterminated_src); TC_RUN(int_and_s); TC_RUN(wide_count_size); TC_RUN(wide_unterminated); return tc_end(); }
