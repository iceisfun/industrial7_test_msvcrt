/* TC
 * id:    loc_neg_create_locale_null_name
 * area:  locale
 * funcs: _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _create_locale (silent on NULL locale name)
 * note:  Win10 triage (class A): observed on Windows 10 22H2 build 19045, _create_locale(LC_ALL, NULL) returns NULL silently: errno stays 0 and the invalid-parameter
 *        handler is NOT invoked (no parameter validation on this path). Assertions record that; stable across 2 runs.
 * note:  Original PROBE: _create_locale(LC_ALL, NULL) returns NULL, sets errno EINVAL and invokes the invalid-parameter handler.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 */
#include "tc.h"
#include <errno.h>
#include <locale.h>

static const char *volatile n_null = NULL;

TC_CASE(null_name)
{
    _locale_t l = _create_locale(LC_ALL, n_null);
    CHECK(l == NULL);
    if (l) _free_locale(l);
}
TC_CASE(null_name_einval_iph)
{
    _locale_t l; int before = tc_iph_count;
    errno = 0; l = _create_locale(LC_ALL, n_null);
    CHECK_EQ_I(errno, 0);                      /* observed: no errno */
    CHECK_EQ_I(tc_iph_count - before, 0);      /* observed: handler not invoked */
    if (l) _free_locale(l);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_name); TC_RUN(null_name_einval_iph); return tc_end(); }
