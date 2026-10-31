/* TC
 * id:    sec_wgetenv_s_basic
 * area:  secure_crt
 * funcs: _wgetenv_s
 * procs: _wputenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs getenv_s, _wgetenv_s, _putenv_s
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

typedef int (__cdecl *wputenv_s_t)(const wchar_t *, const wchar_t *);
static wputenv_s_t p_wputenv_s;
TC_CASE(set_and_get) { wchar_t b[32]; size_t n = 99; if (!p_wputenv_s) TC_SKIP("no _wputenv_s");
    CHECK_EQ_I(p_wputenv_s(L"TC_SEC_WVAR1", L"wide"), 0); CHECK_EQ_I(_wgetenv_s(&n, b, 32, L"TC_SEC_WVAR1"), 0); CHECK_WSTR(b, L"wide"); }
TC_CASE(retsize) { wchar_t b[32]; size_t n = 99; if (!p_wputenv_s) TC_SKIP("no _wputenv_s");
    p_wputenv_s(L"TC_SEC_WVAR1", L"wide"); CHECK_EQ_I(_wgetenv_s(&n, b, 32, L"TC_SEC_WVAR1"), 0); CHECK_EQ_U(n, 5);
    n = 99; CHECK_EQ_I(_wgetenv_s(&n, NULL, 0, L"TC_SEC_WVAR1"), 0); CHECK_EQ_U(n, 5); }
TC_CASE(absent_zero_size) { wchar_t b[8]; size_t n = 99; if (p_wputenv_s) p_wputenv_s(L"TC_SEC_WVAR1", L"");
    b[0] = L'x'; CHECK_EQ_I(_wgetenv_s(&n, b, 8, L"TC_SEC_WVAR1"), 0); CHECK_EQ_U(n, 0); CHECK_EQ_I(b[0], 0); }
int main(void) { tc_begin(); p_wputenv_s = (wputenv_s_t)tc_crt_proc("_wputenv_s");
    TC_RUN(set_and_get); TC_RUN(retsize); TC_RUN(absent_zero_size); return tc_end(); }
