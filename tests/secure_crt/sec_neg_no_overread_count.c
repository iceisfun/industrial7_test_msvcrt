/* TC
 * id:    sec_neg_no_overread_count
 * area:  secure_crt
 * funcs:
 * procs: strncpy_s strncat_s wcsncpy_s wcsncat_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s / strncat_s: at most count characters are copied
 * note:  Source arrays are deliberately unterminated; only count chars may be read/copied.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, strncpy_s, (char*,size_t,const char*,size_t));
TC_DECL_PROC(errno_t, strncat_s, (char*,size_t,const char*,size_t));
TC_DECL_PROC(errno_t, wcsncpy_s, (wchar_t*,size_t,const wchar_t*,size_t));
TC_DECL_PROC(errno_t, wcsncat_s, (wchar_t*,size_t,const wchar_t*,size_t));
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static const char s[8] = {'a','b','c','d','P','O','I','S'};
static const wchar_t w[8] = {L'a', L'b', L'c', L'd', L'P', L'O', L'I', L'S'};
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(strncpy) { char d[8]; memset(d, 'z', sizeof d); setup(); CHECK_EQ_I(tc_p_strncpy_s(d, 8, s, 4), 0); CHECK_STR(d, "abcd"); CHECK_EQ_I(d[4], 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(strncat) { char d[8] = "xy"; setup(); CHECK_EQ_I(tc_p_strncat_s(d, 8, s, 3), 0); CHECK_STR(d, "xyabc"); CHECK_EQ_I(d[5], 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(wcsncpy) { wchar_t d[8]; int i; for (i = 0; i < 8; i++) d[i] = L'z'; setup(); CHECK_EQ_I(tc_p_wcsncpy_s(d, 8, w, 4), 0); CHECK_WSTR(d, L"abcd"); CHECK_EQ_I(d[4], 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(wcsncat) { wchar_t d[8] = L"xy"; setup(); CHECK_EQ_I(tc_p_wcsncat_s(d, 8, w, 3), 0); CHECK_WSTR(d, L"xyabc"); CHECK_EQ_I(d[5], 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(tight_dest) { char d[5]; setup(); CHECK_EQ_I(tc_p_strncpy_s(d, 5, s, 4), 0); CHECK_STR(d, "abcd"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(strncpy_s); TC_LOAD_PROC(strncat_s); TC_LOAD_PROC(wcsncpy_s); TC_LOAD_PROC(wcsncat_s); TC_RUN(strncpy); TC_RUN(strncat); TC_RUN(wcsncpy); TC_RUN(wcsncat); TC_RUN(tight_dest); return tc_end(); }
