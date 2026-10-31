/* TC
 * id:    str_neg_strrev_null_probe
 * area:  string
 * funcs: _strrev
 * procs: 
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strrev (lists no error return)
 * note:  PROBE: _strrev(NULL) returns NULL, errno EINVAL, invalid parameter handler called once. Undocumented; UCRT is believed to validate.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile p_rev)(char *) = _strrev;
TC_CASE(null_probe)
{
    char *r; tc_iph_count = 0; errno = 0;
    r = p_rev(NULL);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); if (!tc_iph_install()) return 3; TC_RUN(null_probe); return tc_end(); }
