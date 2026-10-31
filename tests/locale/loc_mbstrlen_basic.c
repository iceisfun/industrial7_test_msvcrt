/* TC
 * id:    loc_mbstrlen_basic
 * area:  locale
 * funcs: _mbstrlen _mbstrlen_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _mbstrlen, _mbstrlen_l
 * note:  In an SBCS locale counts bytes. Shift-JIS locale cases skip if unavailable.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <locale.h>

static _locale_t jp;
static const char *volatile v_hi = "\x82\xA0" "b\xFF";
static const char *volatile v_jp = "a\x82\xA0" "b";
static const char *volatile v_empty = "";

TC_CASE(c_locale_strlen) { CHECK_EQ_U(_mbstrlen(v_hi), 4); CHECK_EQ_U(_mbstrlen("abc"), 3); }
TC_CASE(dbcs_count)
{
    if (!jp) TC_SKIP("932 locale unavailable");
    CHECK_EQ_U(_mbstrlen_l(v_jp, jp), 3);
}
TC_CASE(empty) { CHECK_EQ_U(_mbstrlen(v_empty), 0); }
TC_CASE(l_variant_independent)
{
    if (!jp) TC_SKIP("932 locale unavailable");
    CHECK_EQ_U(_mbstrlen_l(v_jp, jp), 3);
    CHECK_EQ_U(_mbstrlen(v_jp), 4);   /* global locale is still C */
}
int main(void)
{
    tc_begin();
    jp = _create_locale(LC_ALL, "Japanese_Japan.932");
    TC_RUN(c_locale_strlen); TC_RUN(dbcs_count); TC_RUN(empty); TC_RUN(l_variant_independent);
    if (jp) _free_locale(jp);
    return tc_end();
}
