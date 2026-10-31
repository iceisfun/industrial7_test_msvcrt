/* TC
 * id:    exp_locale_api_exported
 * area:  exports
 * funcs: 
 * procs: setlocale _wsetlocale localeconv _create_locale _free_locale _get_current_locale _configthreadlocale
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs setlocale, _create_locale, _free_locale, _get_current_locale, _configthreadlocale (VS2005+)
 * note:  msvcr80 and _wcreate_locale are covered by exp_probe_locale_msvcr80 / exp_locale_wcreate_exported / exp_probe_wcreate_locale_old.
 *        _configthreadlocale(0) must not fail (-1); its exact default value is probed in exp_probe_configthreadlocale_default.
 */
#include "tc.h"

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef void *(__cdecl *getcur_t)(void);
typedef int (__cdecl *cfg_t)(int);
typedef char *(__cdecl *setlocale_t)(int, const char *);

static const char *const group[] = { "setlocale", "_wsetlocale", "localeconv" };

TC_CASE(setlocale_group)
{
    unsigned i;
    for (i = 0; i < sizeof group / sizeof group[0]; i++)
        if (!tc_crt_proc(group[i])) tc_fail_expr(__FILE__, __LINE__, group[i]);
}
TC_CASE(create_free_locale)
{
    create_t c = (create_t)tc_crt_proc("_create_locale");
    free_t f = (free_t)tc_crt_proc("_free_locale");
    void *loc;
    CHECK(c != NULL); CHECK(f != NULL);
    if (!c || !f) return;
    loc = c(0 /* LC_ALL */, "C");
    CHECK(loc != NULL);
    if (loc) f(loc);
    f(NULL);   /* documented: _free_locale(NULL) is harmless */
}
TC_CASE(get_current_locale_nonnull)
{
    getcur_t g = (getcur_t)tc_crt_proc("_get_current_locale");
    free_t f = (free_t)tc_crt_proc("_free_locale");
    void *loc;
    CHECK(g != NULL);
    if (!g) return;
    loc = g();
    CHECK(loc != NULL);
    (void)f;   /* ownership of the returned object is not documented: deliberately not freed (small leak) */
}
TC_CASE(configthreadlocale_query)
{
    cfg_t p = (cfg_t)tc_crt_proc("_configthreadlocale");
    CHECK(p != NULL);
    if (!p) return;
    CHECK(p(0) != -1);   /* 0 = query only; -1 would mean failure */
}
int main(void)
{
    tc_begin();
    TC_RUN(setlocale_group); TC_RUN(create_free_locale); TC_RUN(get_current_locale_nonnull); TC_RUN(configthreadlocale_query);
    return tc_end();
}
