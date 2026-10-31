/* TC
 * id:    gsf_neg_probe_ftelli64_nolock_null
 * area:  stdio_file
 * funcs: _ftelli64_nolock
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ftelli64 (ftell family parameter validation); the _nolock variants are not individually documented to validate
 * note:  PROBE: the undocumented _nolock entry point validates a NULL stream like the locking one: returns -1, errno EINVAL,
 *        invalid-parameter handler called once (believed; ucrt/VS2005+ sources are believed to use _VALIDATE_RETURN here).
 *        Restricted to >=100: validation in the msvcr80/90 _nolock entry points is doubtful (a NULL deref there would crash).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
__int64 __cdecl _ftelli64_nolock(FILE *);
static FILE *volatile vnull;

TC_CASE(null)
{
    __int64 r;
    errno = 0; tc_iph_count = 0;
    r = _ftelli64_nolock(vnull);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null);
    return tc_end();
}
