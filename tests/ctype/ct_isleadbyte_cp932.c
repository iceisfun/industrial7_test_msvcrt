/* TC
 * id:    ct_isleadbyte_cp932
 * area:  ctype
 * funcs: isleadbyte setlocale ___mb_cur_max_func
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs isleadbyte, setlocale; Code Page 932 lead bytes 0x81-0x9F, 0xE0-0xFC
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static int have;
TC_CASE(setlocale_ok_or_skip) { have = setlocale(LC_ALL, ".932") != NULL; if (!have) TC_SKIP("cp932 locale unavailable"); }
TC_CASE(lead_ranges_true)
{
    static const int v[] = {0x81, 0x9F, 0xE0, 0xFC}; int i;
    if (!have) TC_SKIP("no locale");
    for (i = 0; i < 4; i++) { volatile int c = v[i]; CHECK(isleadbyte(c)); }
}
TC_CASE(non_lead_false)
{
    static const int v[] = {0x00, 0x41, 0x80, 0xA0, 0xDF, 0xFD, 0xFF}; int i;
    if (!have) TC_SKIP("no locale");
    for (i = 0; i < 7; i++) { volatile int c = v[i]; CHECK(!isleadbyte(c)); }
}
TC_CASE(mb_cur_max_is_2) { if (!have) TC_SKIP("no locale"); CHECK_EQ_I(___mb_cur_max_func(), 2); }
TC_CASE(restore_C_locale)
{
    const char *r = setlocale(LC_ALL, "C");
    CHECK(r != NULL);
    CHECK_EQ_I(___mb_cur_max_func(), 1);
    { volatile int c = 0x81; CHECK(!isleadbyte(c)); }
}
int main(void) { tc_begin(); TC_RUN(setlocale_ok_or_skip); TC_RUN(lead_ranges_true); TC_RUN(non_lead_false); TC_RUN(mb_cur_max_is_2); TC_RUN(restore_C_locale); return tc_end(); }
