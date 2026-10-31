/* TC
 * id:    mem_neg_memcpy_overlap_memmove_s_valid
 * area:  memory
 * funcs:
 * procs: memmove_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memmove_s (overlap allowed; ERANGE if count > destsz)
 * note:  Overlapping regions are valid for memmove_s; the size check is count <= destsz irrespective of overlap.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *ms_t)(void *, size_t, const void *, size_t);
static ms_t p_ms;
static void init(unsigned char *b) { int i; for (i = 0; i < 16; i++) b[i] = (unsigned char)(i + 1); }

TC_CASE(overlap_forward_ok)
{
    unsigned char b[16], e[16]; volatile size_t n = 8;
    CHECK(p_ms != NULL); if (!p_ms) return;
    init(b); init(e); memmove(e + 2, e, 8);
    tc_iph_count = 0;
    CHECK_EQ_I(p_ms(b + 2, 8, b, n), 0);
    CHECK_MEM(b, e, 16); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(overlap_backward_ok)
{
    unsigned char b[16], e[16]; volatile size_t n = 8;
    CHECK(p_ms != NULL); if (!p_ms) return;
    init(b); init(e); memmove(e, e + 2, 8);
    CHECK_EQ_I(p_ms(b, 8, b + 2, n), 0);
    CHECK_MEM(b, e, 16);
}
TC_CASE(overlap_count_gt_destsz_erange)
{
    unsigned char b[16], o[16]; volatile size_t n = 8; int r;
    CHECK(p_ms != NULL); if (!p_ms) return;
    init(b); init(o);
    tc_iph_count = 0; errno = 0;
    r = p_ms(b, 4, b + 2, n);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_MEM(b + 4, o + 4, 12);
}
TC_CASE(same_ptr_ok)
{
    unsigned char b[16], o[16]; volatile size_t n = 8;
    CHECK(p_ms != NULL); if (!p_ms) return;
    init(b); init(o);
    CHECK_EQ_I(p_ms(b, 8, b, n), 0);
    CHECK_MEM(b, o, 16);
}
int main(void)
{
    tc_begin();
    p_ms = (ms_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(overlap_forward_ok); TC_RUN(overlap_backward_ok); TC_RUN(overlap_count_gt_destsz_erange); TC_RUN(same_ptr_ok);
    return tc_end();
}
