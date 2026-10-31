/* TC
 * id:    sec_neg_wide_conv_s_args
 * area:  secure_crt
 * funcs:
 * procs: _ultow_s _ltow_s _i64tow_s _ui64tow_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itow_s family: NULL buffer / size 0 / radix outside 2..36 -> EINVAL
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _ltow_s, (long,wchar_t*,size_t,int));
TC_DECL_PROC(errno_t, _ultow_s, (unsigned long,wchar_t*,size_t,int));
TC_DECL_PROC(errno_t, _i64tow_s, (long long,wchar_t*,size_t,int));
TC_DECL_PROC(errno_t, _ui64tow_s, (unsigned long long,wchar_t*,size_t,int));
static wchar_t *volatile v_null;
static volatile size_t v_zero;
static volatile int v_r;
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
#define ALL4(buf, sz, radix) do { \
    errno = 0; CHECK_EQ_I(tc_p__ultow_s(5, buf, sz, radix), EINVAL); CHECK_EQ_I(errno, EINVAL); \
    errno = 0; CHECK_EQ_I(tc_p__ltow_s(5, buf, sz, radix), EINVAL); CHECK_EQ_I(errno, EINVAL); \
    errno = 0; CHECK_EQ_I(tc_p__i64tow_s(5, buf, sz, radix), EINVAL); CHECK_EQ_I(errno, EINVAL); \
    errno = 0; CHECK_EQ_I(tc_p__ui64tow_s(5, buf, sz, radix), EINVAL); CHECK_EQ_I(errno, EINVAL); } while (0)

TC_CASE(null_buffer) { setup(); ALL4(v_null, 10, 10); CHECK_EQ_I(tc_iph_count, 4); }
TC_CASE(size0) { wchar_t b[4] = L"zzz"; setup(); ALL4(b, v_zero, 10); CHECK_WSTR(b, L"zzz"); CHECK_EQ_I(tc_iph_count, 4); }
TC_CASE(radix1) { wchar_t b[16]; setup(); v_r = 1; ALL4(b, 16, v_r); CHECK_EQ_I(tc_iph_count, 4); }
TC_CASE(radix37) { wchar_t b[16]; setup(); v_r = 37; ALL4(b, 16, v_r); CHECK_EQ_I(tc_iph_count, 4); }
TC_CASE(valid_no_handler) { wchar_t b[16]; setup(); CHECK_EQ_I(tc_p__ultow_s(5, b, 16, 10), 0); CHECK_WSTR(b, L"5"); CHECK_EQ_I(tc_p__i64tow_s(-5, b, 16, 10), 0); CHECK_WSTR(b, L"-5"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(_ltow_s); TC_LOAD_PROC(_ultow_s); TC_LOAD_PROC(_i64tow_s); TC_LOAD_PROC(_ui64tow_s); TC_RUN(null_buffer); TC_RUN(size0); TC_RUN(radix1); TC_RUN(radix37); TC_RUN(valid_no_handler); return tc_end(); }
