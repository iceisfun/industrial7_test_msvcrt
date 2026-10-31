/* TC
 * id:    loc_neg_probe_setlocale_composite_short_probe
 * area:  locale
 * funcs: setlocale
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (composite strings)
 * note:  PROBE: believed that a composite string with too few components ("C;C") is rejected by ucrtbase setlocale for LC_ALL, and
 *        that a composite string given for a single category (LC_CTYPE) is rejected too. Not documented explicitly.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile n_short = "C;C";

TC_CASE(all_short) { CHECK(setlocale(LC_ALL, n_short) == NULL); }
TC_CASE(ctype_composite) { CHECK(setlocale(LC_CTYPE, n_short) == NULL); }
TC_CASE(still_c) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
TC_CASE(still_c2) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void)
{
    tc_begin();
    TC_RUN(all_short); TC_RUN(still_c); TC_RUN(ctype_composite); TC_RUN(still_c2);
    setlocale(LC_ALL, "C");
    return tc_end();
}
