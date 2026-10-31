/* TC
 * id:    env_putenv_case_replace
 * area:  env
 * funcs: _putenv getenv
 * procs: __p__environ
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs getenv, _wgetenv ("getenv is not case sensitive in the Windows operating system")
 * note:  Setting an existing variable with different letter case replaces it (one entry), not appends a duplicate.
 *        The case of the stored name after replacement is NOT asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(char ***, __p__environ, (void));
static char *volatile v_set1 = "TCENV_CASE_R=one";
static char *volatile v_set2 = "tcenv_case_r=two";
static char *volatile v_nm_lo = "tcenv_case_r";
static char *volatile v_nm_up = "TCENV_CASE_R";
static char *volatile v_nm_mx = "TcEnv_Case_R";

static int ci_prefix(const char *e, const char *name)
{
    size_t i;
    for (i = 0; name[i]; i++) {
        char a = e[i], b = name[i];
        if (a >= 'a' && a <= 'z') a -= 32;
        if (b >= 'a' && b <= 'z') b -= 32;
        if (a != b) return 0;
    }
    return e[i] == '=';
}
static int count_env(int *matches)
{
    char **env = tc_p___p__environ ? *tc_p___p__environ() : NULL;
    int n = 0, m = 0;
    if (!env) return -1;
    for (; env[n]; n++) if (ci_prefix(env[n], "TCENV_CASE_R")) m++;
    *matches = m;
    return n;
}

TC_CASE(new_value_visible_via_any_case)
{
    CHECK_EQ_I(_putenv(v_set1), 0);
    CHECK_EQ_I(_putenv(v_set2), 0);
    CHECK(getenv(v_nm_lo) != NULL);
    CHECK(getenv(v_nm_up) != NULL);
    CHECK(getenv(v_nm_mx) != NULL);
    if (getenv(v_nm_up)) CHECK_STR(getenv(v_nm_up), "two");
    if (getenv(v_nm_lo)) CHECK_STR(getenv(v_nm_lo), "two");
    if (getenv(v_nm_mx)) CHECK_STR(getenv(v_nm_mx), "two");
    _putenv("TCENV_CASE_R=");
}
TC_CASE(exactly_one_environ_entry_matching_case_insensitively)
{
    int m = -1;
    CHECK_EQ_I(_putenv(v_set1), 0);
    CHECK_EQ_I(_putenv(v_set2), 0);
    CHECK(count_env(&m) > 0);
    CHECK_EQ_I(m, 1);
    _putenv("TCENV_CASE_R=");
}
TC_CASE(entry_count_unchanged_after_replace)
{
    int m = 0, before, after;
    CHECK_EQ_I(_putenv(v_set1), 0);
    before = count_env(&m);
    CHECK_EQ_I(_putenv(v_set2), 0);
    after = count_env(&m);
    CHECK(before > 0);
    CHECK_EQ_I(after, before);
    _putenv("TCENV_CASE_R=");
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(__p__environ);
    CHECK(tc_p___p__environ != NULL);
    TC_RUN(new_value_visible_via_any_case);
    TC_RUN(exactly_one_environ_entry_matching_case_insensitively);
    TC_RUN(entry_count_unchanged_after_replace);
    return tc_end();
}
