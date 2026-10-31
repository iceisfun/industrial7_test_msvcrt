/* TC
 * id:    ct_probe_cp1252_punct_c1
 * area:  ctype
 * funcs: iscntrl isprint isgraph ispunct setlocale
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs setlocale; GetStringTypeA classification
 * note:  PROBE: in English_United States.1252, 0xD7 and 0xF7 are ispunct, and the five undefined
 *        code points 0x81 0x8D 0x8F 0x90 0x9D are iscntrl and not isprint/isgraph (CRT table is built from GetStringTypeA).
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static int have;
TC_CASE(setlocale_ok_or_skip) { have = setlocale(LC_ALL, "English_United States.1252") != NULL; if (!have) TC_SKIP("locale unavailable"); }
TC_CASE(multiply_divide_punct)
{
    volatile int c;
    if (!have) TC_SKIP("no locale");
    c = 0xD7; CHECK(ispunct(c));
    c = 0xF7; CHECK(ispunct(c));
}
TC_CASE(c1_undefined_five_cntrl_not_print)
{
    static const int v[] = {0x81, 0x8D, 0x8F, 0x90, 0x9D}; int i;
    if (!have) TC_SKIP("no locale");
    for (i = 0; i < 5; i++) { volatile int c = v[i]; CHECK(iscntrl(c)); CHECK(!isprint(c)); CHECK(!isgraph(c)); }
}
TC_CASE(restore_C_locale) { CHECK(setlocale(LC_ALL, "C") != NULL); }
int main(void) { tc_begin(); TC_RUN(setlocale_ok_or_skip); TC_RUN(multiply_divide_punct); TC_RUN(c1_undefined_five_cntrl_not_print); TC_RUN(restore_C_locale); return tc_end(); }
