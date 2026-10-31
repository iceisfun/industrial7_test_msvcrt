/* TC
 * id:    gio_neg_mktemp_null_iph
 * area:  lowio
 * funcs: _mktemp _wmktemp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mktemp; MS docs Parameter Validation (EINVAL + invalid parameter handler)
 * note:  PROBE: _mktemp(NULL)/_wmktemp(NULL) returns NULL with errno EINVAL and exactly one invalid-parameter
 *        handler call. MS docs only document this via the generic parameter-validation rules, not on the _mktemp page.
 *        msvcrt.dll excluded: no parameter validation there.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <wchar.h>

TC_CASE(mktemp_null)
{
    char *volatile p = NULL; int before = tc_iph_count;
    errno = 0;
    CHECK(_mktemp(p) == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(wmktemp_null)
{
    wchar_t *volatile p = NULL; int before = tc_iph_count;
    errno = 0;
    CHECK(_wmktemp(p) == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(mktemp_null); TC_RUN(wmktemp_null);
    return tc_end();
}
