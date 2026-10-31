/* TC
 * id:    ct_neg_dbcs_leadbyte_not_char_class
 * area:  ctype
 * funcs: isalpha isprint isleadbyte isdigit isspace isxdigit isupper islower setlocale
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs isleadbyte, setlocale; CP932 lead bytes 0x81-0x9F
 * note:  PROBE: in cp932, lead bytes 0x81..0x9F are flagged lead byte but are not digit/space/xdigit/upper/lower; the
 *        isalpha/isprint==0 part is a separate case (belief, not documented).
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static int have;
TC_CASE(set) { have = setlocale(LC_ALL, ".932") != NULL; if (!have) TC_SKIP("cp932 unavailable"); }
TC_CASE(flags)
{
    int i;
    if (!have) TC_SKIP("no cp932");
    for (i = 0x81; i <= 0x9F; i++) {
        volatile int c = i;
        CHECK(isleadbyte(c) != 0);
        CHECK(isdigit(c) == 0); CHECK(isspace(c) == 0); CHECK(isxdigit(c) == 0);
        CHECK(isupper(c) == 0); CHECK(islower(c) == 0);
    }
}
TC_CASE(alpha_print)
{
    int i;
    if (!have) TC_SKIP("no cp932");
    for (i = 0x81; i <= 0x9F; i++) { volatile int c = i; CHECK(isalpha(c) == 0); CHECK(isprint(c) == 0); }
}
TC_CASE(restore) { CHECK(setlocale(LC_ALL, "C") != NULL); }
int main(void) { tc_begin(); TC_RUN(set); TC_RUN(flags); TC_RUN(alpha_print); TC_RUN(restore); return tc_end(); }
