/* TC
 * id:    sec_neg_wcs_errno_values
 * area:  secure_crt
 * funcs:
 * procs: wcscpy_s wcscat_s wcsncpy_s wcsncat_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcscpy_s etc: ERANGE (34) on too small; errno set
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, wcscpy_s, (wchar_t*,size_t,const wchar_t*));
TC_DECL_PROC(errno_t, wcscat_s, (wchar_t*,size_t,const wchar_t*));
TC_DECL_PROC(errno_t, wcsncpy_s, (wchar_t*,size_t,const wchar_t*,size_t));
TC_DECL_PROC(errno_t, wcsncat_s, (wchar_t*,size_t,const wchar_t*,size_t));
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(cpy_small) { wchar_t d[3]; int r; setup(); r = tc_p_wcscpy_s(d, 3, L"abc"); CHECK_EQ_I(r, 34); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(cat_small) { wchar_t d[4] = L"ab"; int r; setup(); r = tc_p_wcscat_s(d, 4, L"cd"); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ncpy_small) { wchar_t d[3]; int r; setup(); r = tc_p_wcsncpy_s(d, 3, L"abcdef", 3); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ncat_small) { wchar_t d[4] = L"ab"; int r; setup(); r = tc_p_wcsncat_s(d, 4, L"cdef", 4); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(einval_numeric) { wchar_t d[4]; int r; setup(); r = tc_p_wcscpy_s(d, 4, NULL); CHECK_EQ_I(r, 22); CHECK_EQ_I(errno, EINVAL); }

int main(void) { tc_begin(); TC_LOAD_PROC(wcscpy_s); TC_LOAD_PROC(wcscat_s); TC_LOAD_PROC(wcsncpy_s); TC_LOAD_PROC(wcsncat_s); TC_RUN(cpy_small); TC_RUN(cat_small); TC_RUN(ncpy_small); TC_RUN(ncat_small); TC_RUN(einval_numeric); return tc_end(); }
