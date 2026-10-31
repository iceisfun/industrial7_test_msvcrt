/* TC
 * id:    sec_neg_itoa_s_radix_zero
 * area:  secure_crt
 * funcs:
 * procs: _itoa_s _ltoa_s _i64toa_s _ui64toa_s _ultoa_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa_s: radix must be 2..36 else invalid parameter, EINVAL
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _itoa_s, (int,char*,size_t,int));
TC_DECL_PROC(errno_t, _ltoa_s, (long,char*,size_t,int));
TC_DECL_PROC(errno_t, _ultoa_s, (unsigned long,char*,size_t,int));
TC_DECL_PROC(errno_t, _i64toa_s, (long long,char*,size_t,int));
TC_DECL_PROC(errno_t, _ui64toa_s, (unsigned long long,char*,size_t,int));
static volatile int v_r;
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
#define ALL5(radix, expect) do { char b[16] = "zzzz"; int n = 0; \
    errno = 0; CHECK_EQ_I(tc_p__itoa_s(5, b, sizeof b, radix), expect); CHECK_EQ_I(errno, expect); n++; \
    errno = 0; CHECK_EQ_I(tc_p__ltoa_s(5, b, sizeof b, radix), expect); CHECK_EQ_I(errno, expect); n++; \
    errno = 0; CHECK_EQ_I(tc_p__i64toa_s(5, b, sizeof b, radix), expect); CHECK_EQ_I(errno, expect); n++; \
    errno = 0; CHECK_EQ_I(tc_p__ui64toa_s(5, b, sizeof b, radix), expect); CHECK_EQ_I(errno, expect); n++; \
    errno = 0; CHECK_EQ_I(tc_p__ultoa_s(5, b, sizeof b, radix), expect); CHECK_EQ_I(errno, expect); n++; \
    CHECK_EQ_I(tc_iph_count, n); } while (0)

TC_CASE(radix0) { setup(); v_r = 0; ALL5(v_r, EINVAL); }
TC_CASE(radix_minus1) { setup(); v_r = -1; ALL5(v_r, EINVAL); }
TC_CASE(radix_intmax) { setup(); v_r = 0x7fffffff; ALL5(v_r, EINVAL); }
TC_CASE(radix2_ok) { char b[16]; setup(); CHECK_EQ_I(tc_p__itoa_s(5, b, sizeof b, 2), 0); CHECK_STR(b, "101"); CHECK_EQ_I(tc_p__i64toa_s(5, b, sizeof b, 2), 0); CHECK_STR(b, "101");
    CHECK_EQ_I(tc_p__ultoa_s(5, b, sizeof b, 2), 0); CHECK_STR(b, "101"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(radix36_ok) { char b[16]; setup(); CHECK_EQ_I(tc_p__itoa_s(35, b, sizeof b, 36), 0); CHECK_STR(b, "z"); CHECK_EQ_I(tc_p__ui64toa_s(35, b, sizeof b, 36), 0); CHECK_STR(b, "z");
    CHECK_EQ_I(tc_p__ltoa_s(35, b, sizeof b, 36), 0); CHECK_STR(b, "z"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(_itoa_s); TC_LOAD_PROC(_ltoa_s); TC_LOAD_PROC(_ultoa_s); TC_LOAD_PROC(_i64toa_s); TC_LOAD_PROC(_ui64toa_s); TC_RUN(radix0); TC_RUN(radix_minus1); TC_RUN(radix_intmax); TC_RUN(radix2_ok); TC_RUN(radix36_ok); return tc_end(); }
