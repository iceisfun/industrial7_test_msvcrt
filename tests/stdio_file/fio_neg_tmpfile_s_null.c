/* TC
 * id:    fio_neg_tmpfile_s_null
 * area:  stdio_file
 * kind:  negative
 * funcs:
 * procs: tmpfile_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs tmpfile_s: pFilePtr NULL -> EINVAL, invalid parameter handler invoked
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, tmpfile_s, (FILE **));

TC_CASE(null_ptr)
{
    int r, n0 = tc_iph_count;
    if (!tc_p_tmpfile_s) TC_SKIP("tmpfile_s not exported");
    r = tc_p_tmpfile_s(NULL);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(tmpfile_s); CHECK(tc_p_tmpfile_s != NULL); CHECK(tc_iph_install()); TC_RUN(null_ptr); return tc_end(); }
