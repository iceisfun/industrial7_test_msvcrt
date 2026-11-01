/* TC
 * id:    dat_environ_wide_narrow_sync_probe
 * area:  data_exports
 * funcs: _putenv _wputenv
 * procs: __p__environ __p__wenviron
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _putenv/_wputenv (only getenv/_wgetenv behaviour is promised)
 * note:  Win10 22H2 ucrtbase (observed; consistent with the MS docs for _environ/_wenviron): in a main() program _wenviron stays NULL after
 *        _putenv until the first _wgetenv/_wputenv builds the wide environment ("_lazy_cold" case); once it exists (warm case) _putenv keeps it in sync,
 *        and _wputenv keeps _environ in sync. Was a probe guessing the cold case contained the entry.
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

TC_CASE(putenv_then_wenviron_lazy_cold)
{
    if (!p_env || !p_wenv) TC_SKIP("accessors missing");
    CHECK_EQ_I(_putenv("TC_DEWNS_A=1"), 0);
    CHECK(*p_wenv() == NULL);   /* observed on Win10 22H2: the wide environment is created lazily (first _wgetenv/_wputenv), so _putenv alone leaves _wenviron NULL */
    CHECK(!has_w(L"TC_DEWNS_A=1"));
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
    TC_RUN(putenv_then_wenviron_lazy_cold); TC_RUN(putenv_then_wenviron_contains_warm); TC_RUN(wputenv_then_environ_contains);
    return tc_end();
}
