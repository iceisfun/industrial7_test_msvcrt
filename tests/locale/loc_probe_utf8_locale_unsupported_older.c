/* TC
 * id:    loc_probe_utf8_locale_unsupported_older
 * area:  locale
 * funcs: setlocale
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs UTF-8 Support; setlocale (".UTF-8", ".65001")
 * note:  PROBE: diagnostic. Prints whether the UTF-8 locale is available on this machine (absent before Windows 10
 *        1803) and asserts only that the spellings ".UTF-8", ".65001" and "English_United States.UTF-8" agree on
 *        whether they succeed.
 */
#include "tc.h"
#include <locale.h>

static const char * volatile v_loc;
static int ok(const char *n) { const char *r; v_loc = n; r = setlocale(LC_ALL, v_loc); setlocale(LC_ALL, "C"); return r != NULL; }

TC_CASE(utf8_and_65001_agree)
{
    int a = ok(".UTF-8"), b = ok(".65001");
    tc_write(a ? "TC INFO .UTF-8 supported\n" : "TC INFO .UTF-8 NOT supported\n");
    CHECK_EQ_I(a, b);
}
TC_CASE(en_US_dot_utf8_agree)
{
    int a = ok(".UTF-8"), c = ok("English_United States.UTF-8");
    CHECK_EQ_I(a, c);
}
int main(void) { tc_begin(); TC_RUN(utf8_and_65001_agree); TC_RUN(en_US_dot_utf8_agree); setlocale(LC_ALL, "C"); return tc_end(); }
