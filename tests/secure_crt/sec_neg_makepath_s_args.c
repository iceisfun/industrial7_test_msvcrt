/* TC
 * id:    sec_neg_makepath_s_args
 * area:  secure_crt
 * funcs: _makepath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _makepath_s, _wmakepath_s (Error conditions table)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

TC_CASE(null_path_EINVAL) { int r; errno = 0; r = _makepath_s(NULL, 10, "C:", "\\d\\", "n", ".e"); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(size_zero_EINVAL) { char p[16] = "q"; int r; errno = 0; r = _makepath_s(p, 0, "C:", "\\d\\", "n", ".e"); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(handler_counts)
{
    char p[16]; tc_iph_count = 0;
    _makepath_s(NULL, 10, "C:", "\\d\\", "n", ".e");
    _makepath_s(p, 0, "C:", "\\d\\", "n", ".e");
    CHECK_EQ_I(tc_iph_count, 2);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_path_EINVAL); TC_RUN(size_zero_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}
