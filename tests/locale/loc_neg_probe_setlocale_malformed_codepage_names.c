/* TC
 * id:    loc_neg_probe_setlocale_malformed_codepage_names
 * area:  locale
 * funcs: setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs setlocale (locale name grammar "language[_country][.codepage]")
 * note:  Win10 triage (class A, probe -> observed): ".1252x" is ACCEPTED by ucrtbase 19045 (returns "English_United States.1252"); ".-1", ".0x4e4", ".0"
 *        are rejected as believed. Observed on Windows 10 22H2 build 19045.
 *        Original PROBE: malformed code page suffixes (".-1", ".0x4e4", ".1252x", ".0") are believed to be rejected (NULL), leaving "C". One case per name
 *        so a divergence on real Windows is isolated. None of this is spelled out in the docs.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 *        [env] user-default locale en-US, build 19045; not stable on a VM with another user locale/NLS data.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile v_n;
TC_CASE(dot_minus1) { v_n = ".-1"; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
TC_CASE(dot_hex) { v_n = ".0x4e4"; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
/* observed on Win10 22H2 ucrtbase: the code page suffix is parsed atoi-style, so trailing junk after the digits is ignored and ".1252x" selects 1252 */
TC_CASE(trailing_x) { v_n = ".1252x"; CHECK_STR(setlocale(LC_ALL, v_n), "English_United States.1252"); setlocale(LC_ALL, "C"); }
TC_CASE(dot_zero) { v_n = ".0"; CHECK(setlocale(LC_ALL, v_n) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); setlocale(LC_ALL, "C"); }
int main(void)
{
    tc_begin();
    TC_RUN(dot_minus1); TC_RUN(dot_hex); TC_RUN(trailing_x); TC_RUN(dot_zero);
    setlocale(LC_ALL, "C");
    return tc_end();
}
