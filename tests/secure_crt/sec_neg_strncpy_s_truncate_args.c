/* TC
 * id:    sec_neg_strncpy_s_truncate_args
 * area:  secure_crt
 * funcs:
 * procs: strncpy_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s (_TRUNCATE, STRUNCATE, EINVAL)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, strncpy_s, (char*,size_t,const char*,size_t));
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static const char *volatile v_null;
static char *volatile v_dnull;
static volatile size_t v_zero;
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(null_src) { char d[8] = "zzz"; setup(); CHECK_EQ_I(tc_p_strncpy_s(d, sizeof d, v_null, _TRUNCATE), EINVAL); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_dest) { setup(); CHECK_EQ_I(tc_p_strncpy_s(v_dnull, 4, "abc", _TRUNCATE), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size0) { char d[4] = "zzz"; setup(); CHECK_EQ_I(tc_p_strncpy_s(d, v_zero, "ab", _TRUNCATE), EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_STR(d, "zzz"); }
TC_CASE(exact_fit_is_success) { char d[4] = "zzz"; setup(); CHECK_EQ_I(tc_p_strncpy_s(d, sizeof d, "abc", _TRUNCATE), 0); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(one_too_long_truncates) { char d[4] = "zzz"; int r; setup(); r = tc_p_strncpy_s(d, sizeof d, "abcd", _TRUNCATE);
    CHECK_EQ_I(r, STRUNCATE); CHECK_EQ_I(r, 80); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(strncpy_s); TC_RUN(null_src); TC_RUN(null_dest); TC_RUN(size0); TC_RUN(exact_fit_is_success); TC_RUN(one_too_long_truncates); return tc_end(); }
