/* TC
 * id:    dir_neg_fullpath_zero_maxlen
 * area:  dir
 * kind:  negative
 * funcs: _fullpath
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _fullpath (maxLength is the buffer size; parameter validation in VS2005+)
 * note:  Probes _fullpath(buf, "a", 0) with a non-NULL buffer: expected to return NULL without touching buf. errno
 *        (EINVAL vs ERANGE) and whether the invalid-parameter handler runs are recorded only, not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(zero_maxlen)
{
    char buf[16]; char *r; int i, same = 1; volatile const char *p = "a"; volatile size_t z = 0;
    for (i = 0; i < 16; i++) buf[i] = 'X';
    tc_iph_install();
    errno = 0;
    r = _fullpath(buf, (const char *)p, z);
    CHECK(r == NULL);
    for (i = 0; i < 16; i++) if (buf[i] != 'X') same = 0;
    CHECK(same);
    CHECK(errno == EINVAL || errno == ERANGE || errno == 0);
}
int main(void) { tc_begin(); TC_RUN(zero_maxlen); return tc_end(); }
