/* TC
 * id:    env_neg_dupenv_s_args
 * area:  env
 * funcs: _dupenv_s _putenv_s free
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _dupenv_s (EINVAL when buffer or varname is NULL)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(buffer_NULL)
{
    size_t n = 7; int before = tc_iph_count; errno_t e;
    errno = 0; e = _dupenv_s(NULL, &n, "TC_DN1");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - before, 1);
}
TC_CASE(varname_NULL)
{
    char *b = NULL; size_t n = 7; int before = tc_iph_count; errno_t e;
    errno = 0; e = _dupenv_s(&b, &n, NULL);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - before, 1);
    free(b);
}
TC_CASE(errno_EINVAL)
{
    char *b = NULL; size_t n = 0;
    errno = 0; _dupenv_s(NULL, &n, "X"); CHECK_EQ_I(errno, EINVAL);
    errno = 0; _dupenv_s(&b, &n, NULL); CHECK_EQ_I(errno, EINVAL);
    free(b);
}
TC_CASE(buffer_prefilled_not_leaked_on_failure)
{
    /* prefilled with a sentinel; on a NULL varname failure we only require that we can still free
       whatever pointer the buffer holds if it was changed (no crash), and that EINVAL was returned. */
    char *b = NULL; size_t n = 0; errno_t e;
    _putenv_s("TC_DN2", "keep");
    CHECK_EQ_I(_dupenv_s(&b, &n, "TC_DN2"), 0);
    CHECK(b != NULL);
    e = _dupenv_s(&b, &n, NULL);
    CHECK_EQ_I(e, EINVAL);
    free(b);   /* b is either the old allocation or NULL; both are freeable */
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(buffer_NULL); TC_RUN(varname_NULL); TC_RUN(errno_EINVAL); TC_RUN(buffer_prefilled_not_leaked_on_failure);
    return tc_end();
}
