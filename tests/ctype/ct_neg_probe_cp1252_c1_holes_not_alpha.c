/* TC
 * id:    ct_neg_probe_cp1252_c1_holes_not_alpha
 * area:  ctype
 * kind:  negative
 * funcs: isalpha isdigit isupper islower isxdigit toupper tolower setlocale
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs setlocale (language_country.codepage); Windows code page 1252 layout
 * note:  PROBES that the cp1252 positions undefined in the code page (0x81 0x8D 0x8F 0x90 0x9D) are not
 *        alpha/digit/upper/lower/xdigit and are unchanged by toupper/tolower. Skipped if .1252 is unavailable.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static const int holes[5] = { 0x81, 0x8D, 0x8F, 0x90, 0x9D };
static volatile int vc;

TC_CASE(classes_and_case)
{
    int i;
    if (!setlocale(LC_ALL, ".1252")) TC_SKIP("cp1252 unavailable");
    for (i = 0; i < 5; i++) {
        vc = holes[i];
        CHECK_EQ_I(isalpha(vc), 0);
        CHECK_EQ_I(isdigit(vc), 0);
        CHECK_EQ_I(isupper(vc), 0);
        CHECK_EQ_I(islower(vc), 0);
        CHECK_EQ_I(isxdigit(vc), 0);
        CHECK_EQ_I(toupper(vc), holes[i]);
        CHECK_EQ_I(tolower(vc), holes[i]);
    }
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(classes_and_case);
    setlocale(LC_ALL, "C");
    return tc_end();
}
