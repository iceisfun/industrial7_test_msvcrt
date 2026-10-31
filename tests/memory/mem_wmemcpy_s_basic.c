/* TC
 * id:    mem_wmemcpy_s_basic
 * area:  memory
 * funcs:
 * procs: wmemcpy_s wmemmove_s
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs wmemcpy_s, wmemmove_s
 */
#include "tc.h"
#include <errno.h>
#include <wchar.h>
typedef int (__cdecl *wcs_t)(wchar_t *, size_t, const wchar_t *, size_t);
static wcs_t p_wcpy, p_wmov;

TC_CASE(wmemcpy_s_exact_fit)
{
    wchar_t src[4] = {L'a', L'b', 0x4e2d, 0xffff}, dst[4] = {1, 1, 1, 1};
    volatile size_t n = 4;
    CHECK(p_wcpy != NULL); if (!p_wcpy) return;
    CHECK_EQ_I(p_wcpy(dst, n, src, n), 0);
    CHECK_MEM(dst, src, sizeof src);
}
TC_CASE(wmemmove_s_overlap)
{
    wchar_t b[6] = {L'1', L'2', L'3', L'4', L'5', L'6'};
    wchar_t want[6] = {L'1', L'2', L'1', L'2', L'3', L'4'};
    volatile size_t n = 4, ds = 4;
    CHECK(p_wmov != NULL); if (!p_wmov) return;
    CHECK_EQ_I(p_wmov(b + 2, ds, b, n), 0);
    CHECK_MEM(b, want, sizeof b);
}
TC_CASE(size_in_elements_not_bytes)
{
    wchar_t src[8] = {1, 2, 3, 4, 5, 6, 7, 8}, dst[8] = {0}, d2[8] = {0};
    volatile size_t cnt = 8, ds = 8, ds2 = 7;
    int r;
    if (!p_wcpy) return;
    CHECK_EQ_I(p_wcpy(dst, ds, src, cnt), 0);
    CHECK_EQ_I(dst[7], 8);
    CHECK_EQ_I(dst[0], 1);
    /* sizes are in elements: 8 elements into 7 must fail although 7 elements is 14 bytes */
    tc_iph_count = 0;
    r = p_wcpy(d2, ds2, src, cnt);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero_count)
{
    wchar_t src[2] = {L'x', L'y'}, dst[2] = {L'q', L'r'};
    volatile size_t z = 0, ds = 2;
    if (!p_wcpy || !p_wmov) return;
    CHECK_EQ_I(p_wcpy(dst, ds, src, z), 0);
    CHECK_EQ_I(p_wmov(dst, ds, src, z), 0);
    CHECK_EQ_I(dst[0], L'q'); CHECK_EQ_I(dst[1], L'r');
}
int main(void)
{
    tc_begin();
    p_wcpy = (wcs_t)tc_crt_proc("wmemcpy_s");
    p_wmov = (wcs_t)tc_crt_proc("wmemmove_s");
    tc_iph_install();
    TC_RUN(wmemcpy_s_exact_fit); TC_RUN(wmemmove_s_overlap); TC_RUN(size_in_elements_not_bytes); TC_RUN(zero_count);
    return tc_end();
}
