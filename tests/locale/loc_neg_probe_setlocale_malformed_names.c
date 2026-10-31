/* TC
 * id:    loc_neg_probe_setlocale_malformed_names
 * area:  locale
 * funcs: setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (locale name grammar "language[_country][.codepage]")
 * note:  PROBE: malformed punctuation-only names are believed to be rejected (NULL), leaving "C". One case per name
 *        so a divergence on real Windows is isolated. None of this is spelled out in the docs.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile v_n;
TC_CASE(dot) { v_n = "."; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
TC_CASE(dotdot) { v_n = ".."; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
TC_CASE(space) { v_n = " "; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
TC_CASE(c_dot) { v_n = "C."; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
TC_CASE(double_cp) { v_n = "English_United States.1252.1252"; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
int main(void)
{
    tc_begin();
    TC_RUN(dot); TC_RUN(dotdot); TC_RUN(space); TC_RUN(c_dot);
    TC_RUN(double_cp);
    setlocale(LC_ALL, "C");
    return tc_end();
}
