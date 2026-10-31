/* TC
 * id:    sec_neg_xtoa_s_erange_probe
 * area:  secure_crt
 * funcs:
 * procs: _itoa_s _i64toa_s _ui64toa_s _itow_s _i64tow_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s family (table lists EINVAL for too-small buffer; CRT believed to return ERANGE)
 * note:  PROBE: buffer too small for digits+NUL returns exactly ERANGE with errno==ERANGE and the handler once.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _itoa_s, (int,char*,size_t,int));
TC_DECL_PROC(errno_t, _i64toa_s, (long long,char*,size_t,int));
TC_DECL_PROC(errno_t, _ui64toa_s, (unsigned long long,char*,size_t,int));
TC_DECL_PROC(errno_t, _itow_s, (int,wchar_t*,size_t,int));
TC_DECL_PROC(errno_t, _i64tow_s, (long long,wchar_t*,size_t,int));
static volatile int v_i = 12345;
static volatile long long v_ll = 1234567890123LL;
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(itoa) { char b[5]; setup(); CHECK_EQ_I(tc_p__itoa_s(v_i, b, 5, 10), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(i64toa) { char b[5]; setup(); CHECK_EQ_I(tc_p__i64toa_s(v_ll, b, 5, 10), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ui64toa) { char b[5]; setup(); CHECK_EQ_I(tc_p__ui64toa_s((unsigned long long)v_ll, b, 5, 10), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(itow) { wchar_t b[5]; setup(); CHECK_EQ_I(tc_p__itow_s(v_i, b, 5, 10), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(i64tow) { wchar_t b[5]; setup(); CHECK_EQ_I(tc_p__i64tow_s(v_ll, b, 5, 10), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); TC_LOAD_PROC(_itoa_s); TC_LOAD_PROC(_i64toa_s); TC_LOAD_PROC(_ui64toa_s); TC_LOAD_PROC(_itow_s); TC_LOAD_PROC(_i64tow_s); TC_RUN(itoa); TC_RUN(i64toa); TC_RUN(ui64toa); TC_RUN(itow); TC_RUN(i64tow); return tc_end(); }
