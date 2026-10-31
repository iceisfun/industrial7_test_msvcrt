/* TC
 * id:    chr_neg_ungetc_null_stream
 * area:  stdio_char
 * kind:  negative
 * funcs: ungetc
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs ungetc, parameter validation (EINVAL, invalid-parameter handler)
 * note:  ungetc(c, NULL): returns EOF, errno EINVAL, handler invoked once.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

TC_CASE(null_stream)
{
    int r; tc_iph_count = 0; errno = 0;
    r = ungetc('a', NULL);
    CHECK_EQ_I(r, EOF);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_stream); return tc_end(); }
