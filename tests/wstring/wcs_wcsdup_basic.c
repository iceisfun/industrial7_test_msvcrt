/* TC
 * id:    wcs_wcsdup_basic
 * area:  wstring
 * funcs: _wcsdup free
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strdup, _strdup, _wcsdup, _mbsdup
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <malloc.h>

TC_CASE(copy_equal_distinct)
{
    static wchar_t s[] = L"hello"; wchar_t *volatile src = s; wchar_t *d = _wcsdup(src);
    CHECK(d != NULL);
    if (d) { CHECK_WSTR(d, L"hello"); CHECK(d != s); free(d); }
}
TC_CASE(modify_copy)
{
    static wchar_t s[] = L"abc"; wchar_t *volatile src = s; wchar_t *d = _wcsdup(src);
    CHECK(d != NULL); if (!d) return;
    d[0] = L'X';
    CHECK_WSTR(s, L"abc"); CHECK_WSTR(d, L"Xbc");
    free(d);
}
TC_CASE(empty)
{
    static wchar_t s[] = L""; wchar_t *volatile src = s; wchar_t *d = _wcsdup(src);
    CHECK(d != NULL); if (!d) return;
    CHECK_EQ_U(d[0], 0); CHECK(d != s);
    free(d);
}
TC_CASE(long_1000)
{
    static wchar_t s[1001]; wchar_t *volatile src = s; wchar_t *d; int i;
    for (i = 0; i < 1000; i++) s[i] = (wchar_t)(L'a' + i % 26);
    s[1000] = 0;
    d = _wcsdup(src);
    CHECK(d != NULL); if (!d) return;
    CHECK_MEM(d, s, 1001 * sizeof(wchar_t));
    free(d);
}
int main(void) { tc_begin(); TC_RUN(copy_equal_distinct); TC_RUN(modify_copy); TC_RUN(empty); TC_RUN(long_1000); return tc_end(); }
