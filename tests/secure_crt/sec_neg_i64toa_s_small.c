/* TC
 * id:    sec_neg_i64toa_s_small
 * area:  secure_crt
 * funcs:
 * procs: _i64toa_s _ui64toa_s _ltoa_s _ultoa_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa_s family: buffer too small -> failure (ERANGE or EINVAL; exact code in sec_neg_xtoa_s_erange_probe)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _ltoa_s, (long,char*,size_t,int));
TC_DECL_PROC(errno_t, _ultoa_s, (unsigned long,char*,size_t,int));
TC_DECL_PROC(errno_t, _i64toa_s, (long long,char*,size_t,int));
TC_DECL_PROC(errno_t, _ui64toa_s, (unsigned long long,char*,size_t,int));
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
/* too-small buffer: docs table says EINVAL, the CRT is believed to return ERANGE; accept either here, see sec_neg_xtoa_s_erange_probe */
#define CHECK_TOOSMALL(call) do { int r_ = (call); CHECK(r_ == ERANGE || r_ == EINVAL); CHECK_EQ_I(errno, r_); } while (0)
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
static volatile long long v_ll = 1234567890123LL;
static volatile unsigned long long v_ull = 0xFFFFFFFFFFFFFFFFULL;
static volatile long long v_min = (-9223372036854775807LL - 1);
static volatile long v_l = -123456;
static volatile unsigned long v_ul = 4000000000UL;

TC_CASE(i64_small) { char b[5]; setup(); CHECK_TOOSMALL(tc_p__i64toa_s(v_ll, b, 5, 10)); CHECK(errno == ERANGE || errno == EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ui64_max_needs_21) { char b[20]; setup(); CHECK_TOOSMALL(tc_p__ui64toa_s(v_ull, b, 20, 10)); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ui64_max_ok_21) { char b[21]; setup(); CHECK_EQ_I(tc_p__ui64toa_s(v_ull, b, 21, 10), 0); CHECK_STR(b, "18446744073709551615"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(i64_min_needs_21) { char b[20]; setup(); CHECK_TOOSMALL(tc_p__i64toa_s(v_min, b, 20, 10)); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(i64_min_ok_21) { char b[21]; setup(); CHECK_EQ_I(tc_p__i64toa_s(v_min, b, 21, 10), 0); CHECK_STR(b, "-9223372036854775808"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(ltoa_small) { char b[7]; setup(); CHECK_TOOSMALL(tc_p__ltoa_s(v_l, b, 7, 10)); CHECK(errno == ERANGE || errno == EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ultoa_small) { char b[10]; setup(); CHECK_TOOSMALL(tc_p__ultoa_s(v_ul, b, 10, 10)); CHECK(errno == ERANGE || errno == EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ui64_hex_boundary) { char b[3]; setup(); CHECK_TOOSMALL(tc_p__ui64toa_s(255, b, 2, 16)); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__ui64toa_s(255, b, 3, 16), 0); CHECK_STR(b, "ff"); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); TC_LOAD_PROC(_ltoa_s); TC_LOAD_PROC(_ultoa_s); TC_LOAD_PROC(_i64toa_s); TC_LOAD_PROC(_ui64toa_s); TC_RUN(i64_small); TC_RUN(ui64_max_needs_21); TC_RUN(ui64_max_ok_21); TC_RUN(i64_min_needs_21); TC_RUN(i64_min_ok_21); TC_RUN(ltoa_small); TC_RUN(ultoa_small); TC_RUN(ui64_hex_boundary); return tc_end(); }
