/* TC
 * id:    env_neg_failed_put_environ_unchanged
 * area:  env
 * kind:  negative
 * funcs: _putenv _putenv_s _wputenv_s __p__environ
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs _putenv_s (EINVAL on NULL args); that _environ stays untouched is inferred, not stated by the docs
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

char ***__cdecl __p__environ(void);
static const char *volatile nullp = NULL;
static const wchar_t *volatile wnullp = NULL;

static int count(char **e) { int n = 0; if (!e) return -1; while (e[n]) n++; return n; }
static int has_prefix(char **e, const char *p)
{
    int i, j;
    for (i = 0; e && e[i]; i++) { for (j = 0; p[j] && e[i][j] == p[j]; j++) ; if (!p[j]) return 1; }
    return 0;
}

TC_CASE(count_before_after_putenv_null)
{
    int before, after; char **e;
    _putenv("TC_EC0=1");
    before = count(*__p__environ());
    CHECK(before > 0);
    _putenv(nullp);
    e = *__p__environ();
    after = count(e);
    CHECK_EQ_I(before, after);
    CHECK(e != NULL && e[after] == NULL);
    _putenv("TC_EC0=");
}
TC_CASE(count_after_putenv_s_null_value)
{
    int before = count(*__p__environ());
    CHECK_EQ_I(_putenv_s("TC_EC", nullp), EINVAL);
    CHECK(!has_prefix(*__p__environ(), "TC_EC="));
    CHECK_EQ_I(count(*__p__environ()), before);
}
TC_CASE(no_entry_scan_after_wputenv_s_name_null)
{
    int before = count(*__p__environ());
    CHECK_EQ_I(_wputenv_s(wnullp, L"v"), EINVAL);
    CHECK(!has_prefix(*__p__environ(), "TC_EC="));
    CHECK_EQ_I(count(*__p__environ()), before);
}
TC_CASE(array_pointer_still_valid)
{
    char ***pp;
    _putenv(nullp); _putenv_s("TC_EC", nullp);
    pp = __p__environ();
    CHECK(pp != NULL);
    if (pp) CHECK(*pp != NULL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(count_before_after_putenv_null); TC_RUN(count_after_putenv_s_null_value);
    TC_RUN(no_entry_scan_after_wputenv_s_name_null); TC_RUN(array_pointer_still_valid);
    return tc_end();
}
