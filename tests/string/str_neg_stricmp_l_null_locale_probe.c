/* TC
 * id:    str_neg_stricmp_l_null_locale_probe
 * area:  string
 * funcs: _stricmp_l
 * crt:   >=100
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBE: _stricmp_l with a NULL locale uses the current (global) locale instead of failing. Alternative on real Windows: INT_MAX/EINVAL via invalid-parameter handler; this test records which.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile const char *pa, *pb;

TC_CASE(null_locale)
{
    int r; tc_iph_count = 0; errno = 0; pa = "A"; pb = "a";
    r = _stricmp_l((const char *)pa, (const char *)pb, NULL);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_locale); return tc_end(); }
