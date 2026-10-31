/* TC
 * id:    cv_itoa_s_basic
 * area:  conv
 * funcs: _itoa_s _ltoa_s _i64toa_s _ui64toa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _itoa_s, _itoa_s family
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int v_i; static volatile long v_l; static volatile __int64 v_64; static volatile unsigned __int64 v_u64;
static volatile size_t v_n;

TC_CASE(decimal) { char b[32]; int r; v_i = 123; v_n = sizeof b; r = _itoa_s(v_i, b, v_n, 10); CHECK_EQ_I(r, 0); CHECK_STR(b, "123"); }
TC_CASE(exact_fit) { char b[4]; int r; v_i = 123; v_n = 4; r = _itoa_s(v_i, b, v_n, 10); CHECK_EQ_I(r, 0); CHECK_STR(b, "123"); }
TC_CASE(neg_hex_unsigned) { char b[32]; int r; v_i = -255; v_n = sizeof b; r = _itoa_s(v_i, b, v_n, 16); CHECK_EQ_I(r, 0); CHECK_STR(b, "ffffff01"); }
TC_CASE(neg_dec_sign) { char b[32]; int r; v_i = -255; v_n = sizeof b; r = _itoa_s(v_i, b, v_n, 10); CHECK_EQ_I(r, 0); CHECK_STR(b, "-255"); }
TC_CASE(ltoa) { char b[32]; int r; v_l = -123456L; v_n = sizeof b; r = _ltoa_s(v_l, b, v_n, 10); CHECK_EQ_I(r, 0); CHECK_STR(b, "-123456"); v_l = 255; r = _ltoa_s(v_l, b, v_n, 2); CHECK_EQ_I(r, 0); CHECK_STR(b, "11111111"); }
TC_CASE(i64_min) { char b[32]; int r; v_64 = -9223372036854775807LL - 1; v_n = sizeof b; r = _i64toa_s(v_64, b, v_n, 10); CHECK_EQ_I(r, 0); CHECK_STR(b, "-9223372036854775808"); }
TC_CASE(ui64_max_base2) { char b[65]; int r; v_u64 = 0xFFFFFFFFFFFFFFFFULL; v_n = 65; r = _ui64toa_s(v_u64, b, v_n, 2);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(b[63], '1'); CHECK_EQ_U(b[64], 0); CHECK_EQ_U(b[0], '1'); }
TC_CASE(zero_and_base36) { char b[32]; int r; v_i = 0; v_n = sizeof b; r = _itoa_s(v_i, b, v_n, 10); CHECK_EQ_I(r, 0); CHECK_STR(b, "0");
    v_i = 35; r = _itoa_s(v_i, b, v_n, 36); CHECK_EQ_I(r, 0); CHECK_STR(b, "z"); v_i = 36; r = _itoa_s(v_i, b, v_n, 36); CHECK_EQ_I(r, 0); CHECK_STR(b, "10"); }

int main(void)
{
    tc_begin();
    TC_RUN(decimal); TC_RUN(exact_fit); TC_RUN(neg_hex_unsigned); TC_RUN(neg_dec_sign); TC_RUN(ltoa); TC_RUN(i64_min); TC_RUN(ui64_max_base2); TC_RUN(zero_and_base36);
    return tc_end();
}
