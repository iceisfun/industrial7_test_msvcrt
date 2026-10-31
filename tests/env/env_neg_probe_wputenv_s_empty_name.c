/* TC
 * id:    env_neg_probe_wputenv_s_empty_name
 * area:  env
 * kind:  negative
 * funcs: _wputenv_s _wgetenv __p__wenviron
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _putenv_s (empty name not tabulated)
 * note:  PROBE: _wputenv_s(L"", L"v") is rejected with EINVAL and creates nothing (wide mirror of env_neg_probe_putenv_s_empty_name).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

wchar_t ***__cdecl __p__wenviron(void);
static const wchar_t *volatile wempty = L"";

TC_CASE(empty_name_einval)
{
    errno_t e; errno = 0;
    e = _wputenv_s(wempty, L"v");
    CHECK_EQ_I(e, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(nothing_created)
{
    wchar_t **w; int i, bad = 0;
    CHECK(_wgetenv(L"") == NULL);
    w = *__p__wenviron();
    for (i = 0; w && w[i]; i++) if (w[i][0] == L'=' && w[i][1] == L'v' && w[i][2] == 0) bad = 1;
    CHECK(!bad);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(empty_name_einval); TC_RUN(nothing_created);
    return tc_end();
}
