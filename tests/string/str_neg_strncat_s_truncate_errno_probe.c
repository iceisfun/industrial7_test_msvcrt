/* TC
 * id:    str_neg_strncat_s_truncate_errno_probe
 * area:  string
 * funcs: strncat_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncat_s
 * note:  PROBE: whether errno is left untouched (stays 0) when strncat_s returns STRUNCATE. Docs do not state errno for this case.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>

TC_CASE(errno_unchanged)
{
    char d[16] = "abc"; volatile const char *s = "defghij"; int r;
    tc_iph_install(); errno = 0;
    r = strncat_s(d, 8, (const char *)s, _TRUNCATE);
    CHECK_EQ_I(r, 80);
    CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); TC_RUN(errno_unchanged); return tc_end(); }
