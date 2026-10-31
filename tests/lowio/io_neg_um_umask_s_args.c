/* TC
 * id:    io_neg_um_umask_s_args
 * area:  lowio
 * funcs: _umask
 * procs: _umask_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _umask_s: NULL pOldMode or invalid mode -> invalid parameter handler, EINVAL returned
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <sys/stat.h>

TC_DECL_PROC(int, _umask_s, (int, int *));
#define _umask_s tc_p__umask_s

TC_CASE(null_pold)
{
    int before, after;
    CHECK(tc_iph_install());
    before = _umask(0); _umask(before);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_umask_s(0, NULL), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    after = _umask(before);
    CHECK_EQ_I(after, before);
}
TC_CASE(out_of_range_mask)
{
    int old = 0x1234, before, after;
    tc_iph_install();
    before = _umask(0); _umask(before);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_umask_s(0x7777, &old), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    after = _umask(before);
    CHECK_EQ_I(after, before);
}
TC_CASE(valid_roundtrip)
{
    int old = -1, prev = -1, cur = -1;
    tc_iph_install();
    prev = _umask(0);
    _umask(prev);
    tc_iph_count = 0;
    CHECK_EQ_I(_umask_s(_S_IWRITE, &old), 0);
    CHECK_EQ_I(old, prev);
    cur = _umask(prev);
    CHECK_EQ_I(cur, _S_IWRITE);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void) { tc_begin(); TC_LOAD_PROC(_umask_s); if (!tc_p__umask_s) { tc_write("TC FAIL " TC_TEST_ID "::__import _umask_s missing\n"); return 1; } TC_RUN(null_pold); TC_RUN(out_of_range_mask); TC_RUN(valid_roundtrip); return tc_end(); }
