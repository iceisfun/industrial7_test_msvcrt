/* TC
 * id:    sec_dupenv_s_basic
 * area:  secure_crt
 * funcs: _dupenv_s _putenv_s
 * procs: free _wdupenv_s _wputenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _dupenv_s, _wdupenv_s
 * note:  free must be the CRT under test's free (tc_crt_proc), not mingw's
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

typedef void (__cdecl *free_t)(void *);
typedef int (__cdecl *wdupenv_t)(wchar_t **, size_t *, const wchar_t *);
typedef int (__cdecl *wputenv_s_t)(const wchar_t *, const wchar_t *);
static free_t p_free;
TC_CASE(found_copy_and_len) { char *p = NULL; size_t n = 99; if (!p_free) TC_SKIP("no free");
    _putenv_s("TC_SEC_DVAR1", "dupval");
    CHECK_EQ_I(_dupenv_s(&p, &n, "TC_SEC_DVAR1"), 0); CHECK(p != NULL); CHECK_EQ_U(n, 7);
    if (p) { CHECK_STR(p, "dupval"); p[0] = 'X'; CHECK_STR(getenv("TC_SEC_DVAR1"), "dupval"); p_free(p); } }
TC_CASE(absent_returns_0_buffer_null_len_0) { char *p = (char *)1; size_t n = 99; _putenv_s("TC_SEC_DVAR1", "");
    CHECK_EQ_I(_dupenv_s(&p, &n, "TC_SEC_DVAR1"), 0); CHECK(p == NULL); CHECK_EQ_U(n, 0); }
TC_CASE(wdupenv_s_variant) { wchar_t *p = NULL; size_t n = 99; wdupenv_t wd = (wdupenv_t)tc_crt_proc("_wdupenv_s");
    wputenv_s_t wp = (wputenv_s_t)tc_crt_proc("_wputenv_s");
    if (!wd || !wp || !p_free) TC_SKIP("missing export");
    CHECK_EQ_I(wp(L"TC_SEC_DVAR2", L"wdup"), 0);
    CHECK_EQ_I(wd(&p, &n, L"TC_SEC_DVAR2"), 0); CHECK(p != NULL); CHECK_EQ_U(n, 5);
    if (p) { CHECK_WSTR(p, L"wdup"); p_free(p); } wp(L"TC_SEC_DVAR2", L""); }
int main(void) { tc_begin(); p_free = (free_t)tc_crt_proc("free");
    TC_RUN(found_copy_and_len); TC_RUN(absent_returns_0_buffer_null_len_0); TC_RUN(wdupenv_s_variant); return tc_end(); }
