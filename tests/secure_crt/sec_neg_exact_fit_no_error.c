/* TC
 * id:    sec_neg_exact_fit_no_error
 * area:  secure_crt
 * funcs:
 * procs: strcpy_s strcat_s wcscpy_s wcscat_s strncpy_s memcpy_s _itoa_s _i64toa_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s etc: failure only when dest too small for string + NUL
 * note:  Named neg: boundary values that must NOT fail.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, strcpy_s, (char*,size_t,const char*));
TC_DECL_PROC(errno_t, strcat_s, (char*,size_t,const char*));
TC_DECL_PROC(errno_t, strncpy_s, (char*,size_t,const char*,size_t));
TC_DECL_PROC(errno_t, wcscpy_s, (wchar_t*,size_t,const wchar_t*));
TC_DECL_PROC(errno_t, wcscat_s, (wchar_t*,size_t,const wchar_t*));
TC_DECL_PROC(errno_t, memcpy_s, (void*,size_t,const void*,size_t));
TC_DECL_PROC(errno_t, _itoa_s, (int,char*,size_t,int));
TC_DECL_PROC(errno_t, _i64toa_s, (long long,char*,size_t,int));
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
static volatile int v_i = 123;
static volatile long long v_ll = -12;

TC_CASE(strcpy_exact) { char d[4]; setup(); CHECK_EQ_I(tc_p_strcpy_s(d, 4, "abc"), 0); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(strcat_exact) { char d[4] = "a"; setup(); CHECK_EQ_I(tc_p_strcat_s(d, 4, "bc"), 0); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(wcs_exact) { wchar_t d[4], e[4] = L"a"; setup(); CHECK_EQ_I(tc_p_wcscpy_s(d, 4, L"abc"), 0); CHECK_WSTR(d, L"abc");
    CHECK_EQ_I(tc_p_wcscat_s(e, 4, L"bc"), 0); CHECK_WSTR(e, L"abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(strncpy_count_lt) { char d[4]; setup(); CHECK_EQ_I(tc_p_strncpy_s(d, 4, "abcdef", 3), 0); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(memcpy_exact) { char d[4]; setup(); CHECK_EQ_I(tc_p_memcpy_s(d, 4, "wxyz", 4), 0); CHECK_MEM(d, "wxyz", 4); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(itoa_exact) { char b[4]; setup(); CHECK_EQ_I(tc_p__itoa_s(v_i, b, 4, 10), 0); CHECK_STR(b, "123");
    CHECK_EQ_I(tc_p__i64toa_s(v_ll, b, 4, 10), 0); CHECK_STR(b, "-12"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(strcat_s); TC_LOAD_PROC(strncpy_s); TC_LOAD_PROC(wcscpy_s); TC_LOAD_PROC(wcscat_s); TC_LOAD_PROC(memcpy_s); TC_LOAD_PROC(_itoa_s); TC_LOAD_PROC(_i64toa_s); TC_RUN(strcpy_exact); TC_RUN(strcat_exact); TC_RUN(wcs_exact); TC_RUN(strncpy_count_lt); TC_RUN(memcpy_exact); TC_RUN(itoa_exact); return tc_end(); }
