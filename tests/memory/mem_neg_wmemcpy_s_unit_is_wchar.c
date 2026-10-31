/* TC
 * id:    mem_neg_wmemcpy_s_unit_is_wchar
 * area:  memory
 * funcs:
 * procs: wmemcpy_s wmemmove_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wmemcpy_s, wmemmove_s (sizeInWords / count are in wide characters)
 * note:  destsz and count count wchar_t elements, not bytes.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, wmemcpy_s, (wchar_t *, size_t, const wchar_t *, size_t));
TC_DECL_PROC(int, wmemmove_s, (wchar_t *, size_t, const wchar_t *, size_t));
static volatile size_t v_n;

static void rows(int (__cdecl *f)(wchar_t *, size_t, const wchar_t *, size_t))
{
    wchar_t d[8], s[8] = {0x101, 0x202, 0x303, 0x404, 0x505, 0x606, 0x707, 0x808}; int i, r;
    for (i = 0; i < 8; i++) d[i] = 0x7777;
    tc_iph_count = 0;
    v_n = 4; errno = 0; r = f(d, 4, s, v_n);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_MEM(d, s, 4 * sizeof(wchar_t));
    for (i = 4; i < 8; i++) CHECK_EQ_I(d[i], 0x7777);
    v_n = 5; errno = 0; r = f(d, 4, s, v_n);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[4], 0x7777);
    v_n = 8; errno = 0; r = f(d, 4, s, v_n);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 2);
    CHECK_EQ_I(d[4], 0x7777);
}
TC_CASE(wmemcpy_s_units) { if (!tc_p_wmemcpy_s) TC_SKIP("missing"); rows(tc_p_wmemcpy_s); }
TC_CASE(wmemmove_s_units) { if (!tc_p_wmemmove_s) TC_SKIP("missing"); rows(tc_p_wmemmove_s); }
TC_CASE(wmemmove_s_overlap)
{
    wchar_t b[8] = {1, 2, 3, 4, 5, 6, 7, 8}; int r;
    if (!tc_p_wmemmove_s) TC_SKIP("missing");
    tc_iph_count = 0; v_n = 4;
    r = tc_p_wmemmove_s(b + 1, 4, b, v_n);   /* overlapping, count == destsz: ok */
    CHECK_EQ_I(r, 0); CHECK(b[0] == 1 && b[1] == 1 && b[4] == 4 && b[5] == 6);
    v_n = 5;
    r = tc_p_wmemmove_s(b + 1, 4, b, v_n);   /* 5 > 4: ERANGE */
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[5] == 6);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wmemcpy_s); TC_LOAD_PROC(wmemmove_s); CHECK(tc_iph_install());
    TC_RUN(wmemcpy_s_units); TC_RUN(wmemmove_s_units); TC_RUN(wmemmove_s_overlap);
    return tc_end();
}
