/* TC
 * id:    env_wdupenv_s_basic
 * area:  env
 * funcs: _wdupenv_s _wputenv_s free
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0

 * conf:  documented
 * ref:   MS docs _dupenv_s, _wdupenv_s
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(found)
{
    wchar_t *b = NULL; size_t n = 99;
    _wputenv_s(L"TC_WD1", L"wide");
    CHECK_EQ_I(_wdupenv_s(&b, &n, L"TC_WD1"), 0);
    CHECK(b != NULL);
    if (b) CHECK_WSTR(b, L"wide");
    free(b);
}
TC_CASE(not_found_NULL_len0)
{
    wchar_t *b = (wchar_t *)0x1; size_t n = 99;
    CHECK_EQ_I(_wdupenv_s(&b, &n, L"TC_WD_NOSUCH"), 0);
    CHECK(b == NULL); CHECK_EQ_U(n, 0);
}
TC_CASE(non_ascii)
{
    wchar_t *b = NULL; size_t n = 0; wchar_t val[4];
    val[0] = 0x00e9; val[1] = 0x4e2d; val[2] = 0x0416; val[3] = 0;
    _wputenv_s(L"TC_WD2", val);
    CHECK_EQ_I(_wdupenv_s(&b, &n, L"TC_WD2"), 0);
    CHECK(b != NULL);
    if (b) CHECK_WSTR(b, val);
    free(b);
}
TC_CASE(size_in_wchar_incl_nul)
{
    wchar_t *b = NULL; size_t n = 0;
    _wputenv_s(L"TC_WD3", L"12345");
    CHECK_EQ_I(_wdupenv_s(&b, &n, L"TC_WD3"), 0);
    CHECK_EQ_U(n, 6);   /* elements, not bytes */
    free(b);
}
int main(void)
{
    tc_begin();
    TC_RUN(found); TC_RUN(not_found_NULL_len0); TC_RUN(non_ascii); TC_RUN(size_in_wchar_incl_nul);
    return tc_end();
}
