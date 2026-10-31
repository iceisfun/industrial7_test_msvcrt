/* TC
 * id:    loc_neg_probe_setlocale_malformed_codepage_names
 * area:  locale
 * funcs: setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (locale name grammar "language[_country][.codepage]")
 * note:  PROBE: malformed code page suffixes (".-1", ".0x4e4", ".1252x", ".0") are believed to be rejected (NULL), leaving "C". One case per name
 *        so a divergence on real Windows is isolated. None of this is spelled out in the docs.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile v_n;
TC_CASE(dot_minus1) { v_n = ".-1"; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
TC_CASE(dot_hex) { v_n = ".0x4e4"; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
TC_CASE(trailing_x) { v_n = ".1252x"; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
TC_CASE(dot_zero) { v_n = ".0"; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
int main(void)
{
    tc_begin();
    TC_RUN(dot_minus1); TC_RUN(dot_hex); TC_RUN(trailing_x); TC_RUN(dot_zero);
    setlocale(LC_ALL, "C");
    return tc_end();
}
