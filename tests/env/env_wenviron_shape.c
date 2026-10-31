/* TC
 * id:    env_wenviron_shape
 * area:  env
 * funcs: __p__wenviron _wputenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  observed
 * note:  Exact-string storage and entry counts are observed CRT behaviour, not spelled out in the docs.
 * ref:   MS docs _environ, _wenviron, _wputenv (wide environment array of "NAME=value", NULL-terminated, created on first wide call)
 */
#include "tc.h"
#include <stdlib.h>

wchar_t ***__cdecl __p__wenviron(void);

static int wcount(void)
{
    wchar_t **e = *__p__wenviron(); int n = 0;
    if (!e) return -1;
    while (e[n]) n++;
    return n;
}
static int wfind_exact(const wchar_t *s)
{
    wchar_t **e = *__p__wenviron(); int i;
    for (i = 0; e && e[i]; i++) if (tc_wcsequ(e[i], s)) return 1;
    return 0;
}
static int wfind_name(const wchar_t *p)
{
    wchar_t **e = *__p__wenviron(); int i;
    for (i = 0; e && e[i]; i++) {
        const wchar_t *a = e[i], *b = p;
        while (*b && *a == *b) { a++; b++; }
        if (!*b) return 1;
    }
    return 0;
}

TC_CASE(array_nonnull_after_wputenv)
{
    CHECK_EQ_I(_wputenv(L"TC_WES_INIT=1"), 0);
    CHECK(__p__wenviron() != NULL);
    CHECK(*__p__wenviron() != NULL);
    _wputenv(L"TC_WES_INIT=");
}
TC_CASE(new_entry_present_exact)
{
    int before;
    _wputenv(L"TC_WES_A=");
    before = wcount();
    CHECK_EQ_I(_wputenv(L"TC_WES_A=\x00e9wide"), 0);
    CHECK(wfind_exact(L"TC_WES_A=\x00e9wide"));
    CHECK_EQ_I(wcount(), before + 1);
}
TC_CASE(entries_have_equals_and_terminated)
{
    wchar_t **e = *__p__wenviron(); int i, bad = 0;
    CHECK(e != NULL);
    for (i = 0; e && e[i]; i++) {
        const wchar_t *p = e[i]; int eq = 0;
        while (*p) { if (*p == L'=') eq = 1; p++; }
        if (!eq) bad++;
    }
    CHECK_EQ_I(bad, 0);
    CHECK(i >= 1);
}
TC_CASE(delete_removes)
{
    int before;
    CHECK_EQ_I(_wputenv(L"TC_WES_B=zzz"), 0);
    before = wcount();
    CHECK(wfind_name(L"TC_WES_B="));
    CHECK_EQ_I(_wputenv(L"TC_WES_B="), 0);
    CHECK(!wfind_name(L"TC_WES_B="));
    CHECK_EQ_I(wcount(), before - 1);
    _wputenv(L"TC_WES_A=");
}
int main(void)
{
    tc_begin();
    TC_RUN(array_nonnull_after_wputenv); TC_RUN(new_entry_present_exact);
    TC_RUN(entries_have_equals_and_terminated); TC_RUN(delete_removes);
    return tc_end();
}
