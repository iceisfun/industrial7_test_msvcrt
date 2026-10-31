/* TC
 * id:    ct_neg_setlocale_utf8_win7
 * area:  ctype
 * kind:  negative
 * funcs: setlocale
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "UTF-8 support" (setlocale): UTF-8 locales supported starting with the UCRT
 * note:  PROBES that legacy CRTs reject code page 65001 as a setlocale code page. Positive twin: ct_probe_utf8_locale_ucrt.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile n1 = ".65001";
static const char *volatile n2 = ".UTF-8";

TC_CASE(dot_65001_returns_NULL) { CHECK(setlocale(LC_ALL, n1) == NULL); setlocale(LC_ALL, "C"); }
TC_CASE(dot_UTF8_returns_NULL) { CHECK(setlocale(LC_ALL, n2) == NULL); setlocale(LC_ALL, "C"); }
TC_CASE(locale_unchanged_after_failure)
{
    setlocale(LC_ALL, n1); setlocale(LC_ALL, n2);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}

int main(void) { tc_begin(); TC_RUN(dot_65001_returns_NULL); TC_RUN(dot_UTF8_returns_NULL); TC_RUN(locale_unchanged_after_failure); return tc_end(); }
