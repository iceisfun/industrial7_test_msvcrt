/* TC
 * id:    exp_neg_setlocale_invalid_category_probe
 * area:  exports
 * funcs: setlocale
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.11.1.1 (category must be a LC_* macro); MS docs setlocale
 * note:  Probes that an out-of-range category (-1, 7, 100) returns NULL without crashing when an invalid-parameter
 *        handler is installed. The iph count is not asserted.
 */
#include "tc.h"
#include <locale.h>

static volatile int cat;

TC_CASE(category_minus1_null) { tc_iph_install(); cat = -1; CHECK(setlocale(cat, "C") == NULL); }
TC_CASE(category_7_null)      { tc_iph_install(); cat = 7; CHECK(setlocale(cat, "C") == NULL); }
TC_CASE(category_100_null)    { tc_iph_install(); cat = 100; CHECK(setlocale(cat, NULL) == NULL); }

int main(void) { tc_begin(); TC_RUN(category_minus1_null); TC_RUN(category_7_null); TC_RUN(category_100_null); return tc_end(); }
