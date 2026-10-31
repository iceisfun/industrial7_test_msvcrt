/* TC
 * id:    cv_neg_itow_s_errors
 * area:  conv
 * funcs: 
 * procs: _itow_s _i64tow_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itow_s: ERANGE / EINVAL like _itoa_s
 * note:  Probe: wide secure variants assumed to mirror the narrow _itoa_s failure behaviour (buf[0]==0, iph invoked).
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, _itow_s, (int, wchar_t *, size_t, int));
TC_DECL_PROC(int, _i64tow_s, (__int64, wchar_t *, size_t, int));
static wchar_t *volatile v_null; static volatile size_t v_zero; static volatile int v_radix;

TC_CASE(too_small) { wchar_t b[8]; if (!tc_p__itow_s) TC_SKIP("missing"); b[0] = L'x'; tc_iph_count = 0; CHECK_EQ_I(tc_p__itow_s(12345, b, 5, 10), ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_buf) { if (!tc_p__itow_s) TC_SKIP("missing"); tc_iph_count = 0; CHECK_EQ_I(tc_p__itow_s(1, v_null, 10, 10), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_zero) { wchar_t b[4]; if (!tc_p__itow_s) TC_SKIP("missing"); tc_iph_count = 0; CHECK_EQ_I(tc_p__itow_s(1, b, v_zero, 10), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(radix_1) { wchar_t b[16]; if (!tc_p__itow_s) TC_SKIP("missing"); v_radix = 1; tc_iph_count = 0; CHECK_EQ_I(tc_p__itow_s(1, b, 16, v_radix), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(radix_37) { wchar_t b[16]; if (!tc_p__i64tow_s) TC_SKIP("missing"); v_radix = 37; tc_iph_count = 0; CHECK_EQ_I(tc_p__i64tow_s(1, b, 16, v_radix), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(i64_too_small) { wchar_t b[32]; if (!tc_p__i64tow_s) TC_SKIP("missing"); b[0] = L'x'; tc_iph_count = 0; CHECK_EQ_I(tc_p__i64tow_s(-9223372036854775807LL - 1, b, 20, 10), ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    TC_LOAD_PROC(_itow_s); TC_LOAD_PROC(_i64tow_s);
    TC_RUN(too_small); TC_RUN(null_buf); TC_RUN(size_zero); TC_RUN(radix_1); TC_RUN(radix_37); TC_RUN(i64_too_small);
    return tc_end();
}
