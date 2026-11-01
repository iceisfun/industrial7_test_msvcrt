/* TC
 * id:    exp_neg_create_locale_bad_category_errno_probe
 * area:  exports
 * funcs:
 * procs: _create_locale
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _create_locale
 * note:  Probes errno == EINVAL after _create_locale(100, "C") fails (separate from the NULL-return probe).
 * note:  OBSERVED (Win10 22H2 19045; raw "errno == EINVAL got=0 want=22"): a failing _create_locale(100,"C") leaves errno unchanged. Class A.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>

typedef void *(__cdecl *create_t)(int, const char *);
static create_t p_create;
static volatile int cat;

TC_CASE(errno_einval)
{
    if (!p_create) TC_SKIP("not exported");
    tc_iph_install(); cat = 100; errno = 0;
    p_create(cat, "C");
    CHECK_EQ_I(errno, 0);   /* observed: _create_locale leaves errno untouched on a bad category */
}
int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale");
    TC_RUN(errno_einval);
    return tc_end();
}
