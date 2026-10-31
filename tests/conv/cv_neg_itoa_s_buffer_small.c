/* TC
 * id:    cv_neg_itoa_s_buffer_small
 * area:  conv
 * funcs: _itoa_s _ltoa_s _i64toa_s _ui64toa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa_s,_itow_s: buffer not larger than result length is an error; invalid parameter handler invoked
 * note:  Docs table lists EINVAL for too-small buffer while implementations are believed to return ERANGE; so only
 *        'nonzero return' + handler count are asserted here. Exact code and buf[0] are in cv_neg_itoa_s_range_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static volatile int v_i; static volatile long v_l; static volatile __int64 v_64; static volatile unsigned __int64 v_u64;
static volatile size_t v_n;

TC_CASE(itoa_small) { char b[8]; int r; memset(b, 'x', sizeof b); v_i = 12345; v_n = 5; tc_iph_count = 0; errno = 0; r = _itoa_s(v_i, b, v_n, 10);
    CHECK(r != 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(itoa_no_room_for_nul) { char b[8]; int r; memset(b, 'x', sizeof b); v_i = 12345; v_n = 5; tc_iph_count = 0; r = _itoa_s(v_i, b, v_n, 10); CHECK(r != 0);
    v_n = 6; r = _itoa_s(v_i, b, v_n, 10); CHECK_EQ_I(r, 0); CHECK_STR(b, "12345"); }
TC_CASE(neg_sign_counts) { char b[8]; int r; v_i = -123; v_n = 4; tc_iph_count = 0; r = _itoa_s(v_i, b, v_n, 10); CHECK(r != 0); }
TC_CASE(i64_min_size20) { char b[32]; int r; memset(b, 'x', sizeof b); v_64 = -9223372036854775807LL - 1; v_n = 20; tc_iph_count = 0; r = _i64toa_s(v_64, b, v_n, 10);
    CHECK(r != 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ui64_base2_size64) { char b[70]; int r; memset(b, 'x', sizeof b); v_u64 = 0xFFFFFFFFFFFFFFFFULL; v_n = 64; tc_iph_count = 0; r = _ui64toa_s(v_u64, b, v_n, 2);
    CHECK(r != 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ltoa_size1) { char b[4]; int r; memset(b, 'x', sizeof b); v_l = 0; v_n = 1; tc_iph_count = 0; r = _ltoa_s(v_l, b, v_n, 10);
    CHECK(r != 0); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(itoa_small); TC_RUN(itoa_no_room_for_nul); TC_RUN(neg_sign_counts); TC_RUN(i64_min_size20); TC_RUN(ui64_base2_size64); TC_RUN(ltoa_size1);
    return tc_end();
}
