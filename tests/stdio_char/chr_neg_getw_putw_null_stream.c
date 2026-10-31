/* TC
 * id:    chr_neg_getw_putw_null_stream
 * area:  stdio_char
 * funcs: _getw _putw
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _getw, _putw; Parameter validation
 * note:  NULL stream: EOF, errno EINVAL, invalid-parameter handler runs.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
TC_CASE(getw_null)
{
    volatile FILE *p = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_getw((FILE *)p), EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(putw_null)
{
    volatile FILE *p = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_putw(1, (FILE *)p), EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(getw_null); TC_RUN(putw_null); return tc_end(); }
