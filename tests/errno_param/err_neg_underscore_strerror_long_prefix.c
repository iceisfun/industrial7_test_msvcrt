/* TC
 * id:    err_neg_underscore_strerror_long_prefix
 * area:  errno_param
 * kind:  negative
 * funcs: _strerror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _strerror (prefix max 94 chars)
 * note:  PROBE: behaviour with a 200-char prefix; assumed truncation, no crash, bounded result.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(long_prefix)
{
    static char pre[201]; char *r; size_t n; int i;
    for (i = 0; i < 200; i++) pre[i] = 'p';
    pre[200] = 0;
    errno = ENOENT;
    r = _strerror(pre);
    CHECK(r != NULL);
    if (r) {
        n = strlen(r);
        CHECK(n > 0);
        CHECK(n <= 150);
    }
}
int main(void)
{
    tc_begin();
    TC_RUN(long_prefix);
    return tc_end();
}
