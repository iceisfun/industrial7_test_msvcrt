/* TC
 * id:    exp_neg_ucrt_sbh_apis_probe
 * area:  exports
 * funcs: 
 * procs: _set_sbh_threshold _get_sbh_threshold __sbh_heap_init
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: _set_sbh_threshold removed in VS2010 / UCRT
 * note:  PROBE: small-block-heap APIs are gone from ucrtbase.dll.
 */
#include "tc.h"

static void present(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = not exported */
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = unexpectedly exported */
}

static const char *const g0[] = { "_set_sbh_threshold", NULL };
static const char *const g1[] = { "_get_sbh_threshold", NULL };
static const char *const g2[] = { "__sbh_heap_init", NULL };

TC_CASE(n0) { absent(g0); }
TC_CASE(n1) { absent(g1); }
TC_CASE(n2) { absent(g2); }

int main(void)
{
    tc_begin();
    TC_RUN(n0); TC_RUN(n1); TC_RUN(n2);
    return tc_end();
}
