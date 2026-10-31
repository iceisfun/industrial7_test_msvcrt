/* TC
 * id:    env_environ_all_crt_pointer
 * area:  env
 * funcs: _putenv
 * procs: __p__environ
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _environ; __p__environ is the accessor used by <stdlib.h> for _environ in DLL CRTs
 * note:  Resolved at run time because libmsvcr80.a lacks the symbol.
 */
#include "tc.h"
#include <stdlib.h>

typedef char ***(__cdecl *pe_t)(void);
static pe_t p_environ;

TC_CASE(exported) { CHECK(p_environ != NULL); }
TC_CASE(same_pointer)
{
    if (!p_environ) TC_SKIP("not exported");
    CHECK(p_environ() != NULL);
    CHECK_EQ_P(p_environ(), p_environ());
}
TC_CASE(deref_nonnull)
{
    if (!p_environ) TC_SKIP("not exported");
    CHECK(*p_environ() != NULL);
    CHECK(**p_environ() != NULL);  /* a Windows process always has at least one variable */
}
TC_CASE(after_putenv)
{
    char ***pp; char **e; int found = 0, i;
    if (!p_environ) TC_SKIP("not exported");
    pp = p_environ();
    CHECK_EQ_I(_putenv("TC_EAP_V=1"), 0);
    CHECK_EQ_P(p_environ(), pp);
    e = *p_environ();
    CHECK(e != NULL);
    for (i = 0; e && e[i]; i++) if (tc_strequ(e[i], "TC_EAP_V=1")) found = 1;
    CHECK(found);
    _putenv("TC_EAP_V=");
}
int main(void)
{
    tc_begin();
    p_environ = (pe_t)tc_crt_proc("__p__environ");
    TC_RUN(exported); TC_RUN(same_pointer); TC_RUN(deref_nonnull); TC_RUN(after_putenv);
    return tc_end();
}
