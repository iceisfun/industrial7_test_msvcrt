/* TC
 * id:    ct_neg_probe_setlocale_bad_category
 * area:  ctype
 * funcs: setlocale
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (category must be LC_ALL..LC_TIME)
 * note:  PROBE: setlocale with an out-of-range category returns NULL and leaves the C locale. On msvcr80+ this
 *        is a validated parameter (invalid-parameter handler is called with EINVAL); the handler is replaced so
 *        the process survives. The handler count and errno are not asserted (unspecified by docs).
 */
#include "tc.h"
#include <locale.h>

static volatile int cat = 12345;

TC_CASE(bad_category_returns_null)
{
    tc_iph_install();
    setlocale(LC_ALL, "C");
    CHECK(setlocale(cat, "C") == NULL);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(bad_category_returns_null);
    return tc_end();
}
