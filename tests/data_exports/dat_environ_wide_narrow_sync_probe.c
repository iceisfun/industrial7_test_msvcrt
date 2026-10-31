/* TC
 * id:    dat_environ_wide_narrow_sync_probe
 * area:  data_exports
 * funcs: _putenv _wputenv
 * procs: __p__environ __p__wenviron
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _putenv/_wputenv (only getenv/_wgetenv behaviour is promised)
 * note:  PROBE: after _putenv in a main() program, does _wenviron contain the wide form of the entry, and after _wputenv does
 *        _environ contain the narrow form? Guess: yes, the CRT keeps the two arrays in sync once both exist. Cases with
 *        "_cold" use a fresh process state (wide env not yet touched by this test); "_warm" first calls _wgetenv to make the
 *        wide environment exist.
 */
#include "tc.h"
#include <stdlib.h>

typedef char ***(__cdecl *pe_t)(void);
typedef wchar_t ***(__cdecl *pw_t)(void);
static pe_t p_env; static pw_t p_wenv;

static int has_n(const char *s)
{ char **e = *p_env(); int i; for (i = 0; e && e[i]; i++) if (tc_strequ(e[i], s)) return 1; return 0; }
static int has_w(const wchar_t *s)
{ wchar_t **e = *p_wenv(); int i; for (i = 0; e && e[i]; i++) if (tc_wcsequ(e[i], s)) return 1; return 0; }

TC_CASE(putenv_then_wenviron_contains_cold)
{
    if (!p_env || !p_wenv) TC_SKIP("accessors missing");
    CHECK_EQ_I(_putenv("TC_DEWNS_A=1"), 0);
    CHECK(has_w(L"TC_DEWNS_A=1"));
}
TC_CASE(putenv_then_wenviron_contains_warm)
{
    if (!p_env || !p_wenv) TC_SKIP("accessors missing");
    (void)_wgetenv(L"TC_DEWNS_NONE");
    CHECK_EQ_I(_putenv("TC_DEWNS_B=2"), 0);
    CHECK(has_w(L"TC_DEWNS_B=2"));
}
TC_CASE(wputenv_then_environ_contains)
{
    if (!p_env || !p_wenv) TC_SKIP("accessors missing");
    CHECK_EQ_I(_wputenv(L"TC_DEWNS_C=3"), 0);
    CHECK(has_n("TC_DEWNS_C=3"));
}

int main(void)
{
    tc_begin();
    p_env = (pe_t)tc_crt_proc("__p__environ"); p_wenv = (pw_t)tc_crt_proc("__p__wenviron");
    TC_RUN(putenv_then_wenviron_contains_cold); TC_RUN(putenv_then_wenviron_contains_warm); TC_RUN(wputenv_then_environ_contains);
    return tc_end();
}
