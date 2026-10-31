/* TC
 * id:    sec_neg_wide_conv_s_small
 * area:  secure_crt
 * funcs:
 * procs: _itow_s _ultow_s _i64tow_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itow_s: sizeInWords counts wchar_t; too small -> failure (ERANGE or EINVAL)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _itow_s, (int,wchar_t*,size_t,int));
TC_DECL_PROC(errno_t, _ultow_s, (unsigned long,wchar_t*,size_t,int));
TC_DECL_PROC(errno_t, _i64tow_s, (long long,wchar_t*,size_t,int));
static volatile int v_i = 12345;
static volatile int v_n = -12;
static volatile long long v_min = (-9223372036854775807LL - 1);
/* too-small buffer: docs table says EINVAL, the CRT is believed to return ERANGE; accept either here, see sec_neg_xtoa_s_erange_probe */
#define CHECK_TOOSMALL(call) do { int r_ = (call); CHECK(r_ == ERANGE || r_ == EINVAL); CHECK_EQ_I(errno, r_); } while (0)
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(five_digits_size5) { wchar_t b[5]; setup(); CHECK_TOOSMALL(tc_p__itow_s(v_i, b, 5, 10)); CHECK(errno == ERANGE || errno == EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(five_digits_size6) { wchar_t b[6]; setup(); CHECK_EQ_I(tc_p__itow_s(v_i, b, 6, 10), 0); CHECK_WSTR(b, L"12345"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(neg_size3) { wchar_t b[3]; setup(); CHECK_TOOSMALL(tc_p__itow_s(v_n, b, 3, 10)); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(i64min_size20) { wchar_t b[20]; setup(); CHECK_TOOSMALL(tc_p__i64tow_s(v_min, b, 20, 10)); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ultow_size) { wchar_t b[10]; setup(); CHECK_TOOSMALL(tc_p__ultow_s(4000000000UL, b, 10, 10)); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(oversize_bytes_ok) { wchar_t b[40]; setup(); CHECK_EQ_I(tc_p__itow_s(v_i, b, sizeof b, 10), 0); CHECK_WSTR(b, L"12345"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(_itow_s); TC_LOAD_PROC(_ultow_s); TC_LOAD_PROC(_i64tow_s); TC_RUN(five_digits_size5); TC_RUN(five_digits_size6); TC_RUN(neg_size3); TC_RUN(i64min_size20); TC_RUN(ultow_size); TC_RUN(oversize_bytes_ok); return tc_end(); }
