/* TC
 * id:    env_neg_wdupenv_s_args
 * area:  env
 * funcs: _wdupenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _dupenv_s, _wdupenv_s
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(buffer_NULL_EINVAL)
{
    size_t n = 5; errno_t e;
    errno = 0; e = _wdupenv_s(NULL, &n, L"PATH");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(varname_NULL_EINVAL)
{
    wchar_t *b = NULL; size_t n = 5; errno_t e;
    errno = 0; e = _wdupenv_s(&b, &n, NULL);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    free(b);
}
TC_CASE(handler_counts)
{
    wchar_t *b = NULL; size_t n = 0; int before = tc_iph_count;
    _wdupenv_s(NULL, &n, L"PATH");
    CHECK_EQ_I(tc_iph_count - before, 1);
    _wdupenv_s(&b, &n, NULL);
    CHECK_EQ_I(tc_iph_count - before, 2);
    free(b);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no handler API\n"); return tc_end(); }
    TC_RUN(buffer_NULL_EINVAL); TC_RUN(varname_NULL_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}
