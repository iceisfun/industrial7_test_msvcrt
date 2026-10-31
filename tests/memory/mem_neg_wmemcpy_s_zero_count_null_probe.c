/* TC
 * id:    mem_neg_wmemcpy_s_zero_count_null_probe
 * area:  memory
 * funcs:
 * procs: wmemcpy_s wmemmove_s
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wmemcpy_s, wmemmove_s (count 0 with NULL pointers not specified)
 * note:  PROBE: return value and handler count of count==0 with NULL dest / NULL src combinations. memcpy_s is documented to
 *        accept (dest, destsz, NULL, 0) as success; the wide versions are believed to behave the same. Observations are logged; the hard
 *        assertion is only that a nonzero return equals errno and that every call returns 0 or EINVAL.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *wcp_t)(wchar_t *, size_t, const wchar_t *, size_t);
static wcp_t p_cpy, p_mov;
static wchar_t d[4];
static const wchar_t s[4] = { 1, 2, 3, 4 };
static wchar_t *volatile v_nd = NULL;
static const wchar_t *volatile v_ns = NULL;
static volatile size_t v_zero = 0;

static void one(wcp_t f, const char *nm, wchar_t *dd, size_t dsz, const wchar_t *ss)
{
    char b[24]; int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = f(dd, dsz, ss, v_zero);
    tc_write("TC NOTE " TC_TEST_ID " "); tc_write(nm); tc_write(" ret="); tc_i64(r, b); tc_write(b);
    tc_write(" iph="); tc_i64(tc_iph_count, b); tc_write(b); tc_write("\n");
    CHECK(r == 0 || r == EINVAL);
    if (r) CHECK_EQ_I(errno, r);
}
TC_CASE(cpy_null_src) { one(p_cpy, "cpy(d,4,NULL,0)", d, 4, v_ns); }
TC_CASE(cpy_null_dest) { one(p_cpy, "cpy(NULL,0,s,0)", v_nd, 0, s); }
TC_CASE(cpy_null_both) { one(p_cpy, "cpy(NULL,0,NULL,0)", v_nd, 0, v_ns); }
TC_CASE(mov_null_src) { one(p_mov, "mov(d,4,NULL,0)", d, 4, v_ns); }
TC_CASE(mov_null_dest) { one(p_mov, "mov(NULL,0,s,0)", v_nd, 0, s); }
TC_CASE(mov_null_both) { one(p_mov, "mov(NULL,0,NULL,0)", v_nd, 0, v_ns); }
int main(void)
{
    tc_begin();
    p_cpy = (wcp_t)tc_crt_proc("wmemcpy_s"); p_mov = (wcp_t)tc_crt_proc("wmemmove_s");
    tc_iph_install();
    TC_RUN(cpy_null_src); TC_RUN(cpy_null_dest); TC_RUN(cpy_null_both);
    TC_RUN(mov_null_src); TC_RUN(mov_null_dest); TC_RUN(mov_null_both);
    return tc_end();
}
