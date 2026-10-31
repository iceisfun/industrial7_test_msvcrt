/* TC
 * id:    mem_neg_memicmp_l_null_locale_count_zero_probe
 * area:  memory
 * funcs:
 * procs: _memicmp_l
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _memicmp, _memicmp_l (locale NULL means current locale)
 * note:  PROBE: (1) NULL locale behaves as the current ("C") locale: mismatch gives a negative/positive sign and no handler call.
 *        (2) _memicmp_l(NULL, NULL, 0, NULL) returns 0 without invoking the handler (believed from "count == 0 || (buf1 && buf2)").
 */
#include "tc.h"

typedef int (__cdecl *mi_t)(const void *, const void *, size_t, void *);
static mi_t p_mil;
static const void *volatile v_null = NULL;
static volatile size_t v_zero = 0, v_three = 3;

TC_CASE(null_locale_mismatch_sign)
{
    CHECK(p_mil != NULL); if (!p_mil) return;
    tc_iph_count = 0;
    CHECK(p_mil("abc", "abd", v_three, NULL) < 0);
    CHECK(p_mil("ABD", "abc", v_three, NULL) > 0);
    CHECK_EQ_I(p_mil("ABC", "abc", v_three, NULL), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_buffers_count_zero)
{
    CHECK(p_mil != NULL); if (!p_mil) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_mil(v_null, v_null, v_zero, NULL), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_mil = (mi_t)tc_crt_proc("_memicmp_l");
    tc_iph_install();
    TC_RUN(null_locale_mismatch_sign); TC_RUN(null_buffers_count_zero);
    return tc_end();
}
