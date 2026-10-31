/* TC
 * id:    sec_neg_wcsncpy_s_truncate_args
 * area:  secure_crt
 * funcs:
 * procs: wcsncpy_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcsncpy_s (EINVAL conditions)
 * note:  wcscat_s/wcsncat_s size-0 and unterminated-dest cases moved to sec_neg_wcscat_s_dest_probe
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, wcsncpy_s, (wchar_t*,size_t,const wchar_t*,size_t));
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static const wchar_t *volatile v_null;
static volatile size_t v_zero;
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(ncpy_size0) { wchar_t d[4] = L"zzz"; setup(); CHECK_EQ_I(tc_p_wcsncpy_s(d, v_zero, L"abc", 3), EINVAL); CHECK_WSTR(d, L"zzz"); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ncpy_trunc_null_src) { wchar_t d[8] = L"zzz"; setup(); CHECK_EQ_I(tc_p_wcsncpy_s(d, 8, v_null, _TRUNCATE), EINVAL); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ncpy_trunc_exact_fit) { wchar_t d[4] = L"zzz"; setup(); CHECK_EQ_I(tc_p_wcsncpy_s(d, 4, L"abc", _TRUNCATE), 0); CHECK_WSTR(d, L"abc"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(wcsncpy_s); TC_RUN(ncpy_size0); TC_RUN(ncpy_trunc_null_src); TC_RUN(ncpy_trunc_exact_fit); return tc_end(); }
