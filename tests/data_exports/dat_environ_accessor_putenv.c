/* TC
 * id:    dat_environ_accessor_putenv
 * area:  data_exports
 * funcs: _putenv
 * procs: __p__environ
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _environ, _putenv ("NAME=" with empty value removes the variable)
 * note:  Does not assume any pre-existing environment variable.
 */
#include "tc.h"
#include <stdlib.h>

typedef char ***(__cdecl *pe_t)(void);
static pe_t p_environ;

static int find(const char *entry)
{
    char **e = *p_environ(); int i;
    for (i = 0; e && e[i]; i++) if (tc_strequ(e[i], entry)) return 1;
    return 0;
}
static int find_name(const char *prefix)  /* entry starting with prefix */
{
    char **e = *p_environ(); int i, j;
    for (i = 0; e && e[i]; i++) { for (j = 0; prefix[j] && e[i][j] == prefix[j]; j++) ; if (!prefix[j]) return 1; }
    return 0;
}

TC_CASE(environ_ptr_nonnull)
{
    CHECK(p_environ != NULL);
    if (!p_environ) return;
    CHECK(p_environ() != NULL);
    CHECK_EQ_I(_putenv("TC_DAEP_INIT=1"), 0);
    CHECK(*p_environ() != NULL);
    _putenv("TC_DAEP_INIT=");
}
TC_CASE(environ_null_terminated)
{
    char **e; int i, n = 0;
    if (!p_environ) TC_SKIP("not exported");
    e = *p_environ();
    CHECK(e != NULL);
    for (i = 0; e && e[i] && i < 100000; i++) { CHECK(e[i] != NULL); n++; }
    CHECK(n < 100000);
    CHECK(e == NULL || e[n] == NULL);
}
TC_CASE(putenv_adds_entry_visible)
{
    if (!p_environ) TC_SKIP("not exported");
    CHECK(!find_name("TC_DAEP_ADD="));
    CHECK_EQ_I(_putenv("TC_DAEP_ADD=hello"), 0);
    CHECK(find("TC_DAEP_ADD=hello"));
}
TC_CASE(putenv_empty_value_removes_entry)
{
    if (!p_environ) TC_SKIP("not exported");
    CHECK_EQ_I(_putenv("TC_DAEP_RM=x"), 0);
    CHECK(find("TC_DAEP_RM=x"));
    CHECK_EQ_I(_putenv("TC_DAEP_RM="), 0);
    CHECK(!find_name("TC_DAEP_RM="));
}
TC_CASE(entry_form_name_equals_value)
{
    if (!p_environ) TC_SKIP("not exported");
    CHECK_EQ_I(_putenv("TC_DAEP_FORM=a=b c"), 0);
    CHECK(find("TC_DAEP_FORM=a=b c"));
    _putenv("TC_DAEP_FORM=");
    _putenv("TC_DAEP_ADD=");
}

int main(void)
{
    tc_begin();
    p_environ = (pe_t)tc_crt_proc("__p__environ");
    TC_RUN(environ_ptr_nonnull); TC_RUN(environ_null_terminated); TC_RUN(putenv_adds_entry_visible);
    TC_RUN(putenv_empty_value_removes_entry); TC_RUN(entry_form_name_equals_value);
    return tc_end();
}
