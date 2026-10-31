/* TC
 * id:    exp_neg_create_locale_null_name_probe
 * area:  exports
 * funcs:
 * procs: _create_locale
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale
 * note:  Probes that a NULL locale name returns NULL (no crash) with an invalid-parameter handler installed.
 */
#include "tc.h"
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
static create_t p_create;
static const char *volatile nm;

TC_CASE(null_name_null)
{
    if (!p_create) TC_SKIP("_create_locale not exported");
    tc_iph_install(); nm = NULL;
    CHECK(p_create(LC_ALL, nm) == NULL);
}
int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale");
    TC_RUN(null_name_null);
    return tc_end();
}
