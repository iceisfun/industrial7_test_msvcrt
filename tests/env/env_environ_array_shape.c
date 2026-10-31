/* TC
 * id:    env_environ_array_shape
 * area:  env
 * funcs: __p__environ _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  observed
 * note:  Exact-string storage and entry counts are observed CRT behaviour, not spelled out in the docs.
 * ref:   MS docs _environ, _wenviron, environ (array of pointers to "name=value" strings, NULL-terminated)
 */
#include "tc.h"
#include <stdlib.h>

char ***__cdecl __p__environ(void);

static int count(void)
{
    char **e = *__p__environ(); int n = 0;
    if (!e) return -1;
    while (e[n]) n++;
    return n;
}
static int has_exact(const char *s)
{
    char **e = *__p__environ(); int i;
    if (!e) return 0;
    for (i = 0; e[i]; i++) if (tc_strequ(e[i], s)) return 1;
    return 0;
}
static int has_name(const char *prefix)  /* "NAME=" */
{
    char **e = *__p__environ(); int i;
    if (!e) return 0;
    for (i = 0; e[i]; i++) {
        const char *a = e[i], *b = prefix;
        while (*b && *a == *b) { a++; b++; }
        if (!*b) return 1;
    }
    return 0;
}

TC_CASE(pointer_nonnull)
{
    char ***pp = __p__environ();
    CHECK(pp != NULL);
    if (pp) CHECK(*pp != NULL);
}
TC_CASE(entries_have_equals)
{
    char **e = *__p__environ(); int i, n = 0;
    CHECK(e != NULL);
    for (i = 0; e && e[i]; i++) {
        const char *p = e[i]; int eq = 0;
        while (*p) { if (*p == '=') eq = 1; p++; }
        if (!eq) n++;
    }
    CHECK_EQ_I(n, 0);
}
TC_CASE(terminated)
{
    CHECK(count() >= 0);
}
TC_CASE(put_adds_exact)
{
    int before = count();
    CHECK_EQ_I(_putenv("TC_ARR_X=v1"), 0);
    CHECK(has_exact("TC_ARR_X=v1"));
    CHECK_EQ_I(count(), before + 1);
    CHECK_EQ_I(_putenv("TC_ARR_X=second"), 0);
    CHECK(!has_exact("TC_ARR_X=v1"));
    CHECK(has_exact("TC_ARR_X=second"));
    CHECK_EQ_I(count(), before + 1);
    _putenv("TC_ARR_X=");
}
TC_CASE(delete_removes)
{
    int before;
    CHECK_EQ_I(_putenv("TC_ARR_Y=zzz"), 0);
    before = count();
    CHECK(has_name("TC_ARR_Y="));
    CHECK_EQ_I(_putenv("TC_ARR_Y="), 0);
    CHECK(!has_name("TC_ARR_Y="));
    CHECK_EQ_I(count(), before - 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(pointer_nonnull); TC_RUN(entries_have_equals); TC_RUN(terminated);
    TC_RUN(put_adds_exact); TC_RUN(delete_removes);
    return tc_end();
}
