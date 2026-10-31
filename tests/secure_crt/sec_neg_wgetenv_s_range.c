/* TC
 * id:    sec_neg_wgetenv_s_range
 * area:  secure_crt
 * funcs: _wgetenv_s
 * procs: _wputenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs getenv_s, _wgetenv_s
 * note:  variable TC_SEC_NEG2; null name case is in sec_neg_wgetenv_s_nullname_probe
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
typedef int (__cdecl *wputenv_s_t)(const wchar_t *, const wchar_t *);
static volatile size_t v_sz;
static wputenv_s_t p_wputenv_s;
TC_CASE(small_buffer_ERANGE_retsize) { wchar_t b[3]; size_t n = 0; if (!p_wputenv_s) TC_SKIP("no _wputenv_s");
    CHECK_EQ_I(p_wputenv_s(L"TC_SEC_NEG2", L"abcdef"), 0); tc_iph_install(); v_sz = 3;
    CHECK_EQ_I(_wgetenv_s(&n, b, v_sz, L"TC_SEC_NEG2"), ERANGE); CHECK_EQ_U(n, 7); }
TC_CASE(null_retsize_EINVAL) { wchar_t b[8]; if (!p_wputenv_s) TC_SKIP("no _wputenv_s"); p_wputenv_s(L"TC_SEC_NEG2", L"abc");
    tc_iph_install(); tc_iph_count = 0; v_sz = 8; CHECK_EQ_I(_wgetenv_s(NULL, b, v_sz, L"TC_SEC_NEG2"), EINVAL); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); p_wputenv_s = (wputenv_s_t)tc_crt_proc("_wputenv_s");
    TC_RUN(small_buffer_ERANGE_retsize); TC_RUN(null_retsize_EINVAL);
    if (p_wputenv_s) p_wputenv_s(L"TC_SEC_NEG2", L""); return tc_end(); }
