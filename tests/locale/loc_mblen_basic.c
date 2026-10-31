/* TC
 * id:    loc_mblen_basic
 * area:  locale
 * funcs: mblen _mblen_l setlocale
 * procs: _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.7.1; MS docs mblen
 * note:  DBCS cases skip if .932 is rejected.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>

TC_DECL_PROC(_locale_t, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (_locale_t));
static const char * volatile v_np = NULL;
static unsigned char hira[3] = { 0x82, 0xA0, 0 };

TC_CASE(ascii_1) { const char *s = "abc"; CHECK_EQ_I(mblen(s, 3), 1); }
TC_CASE(nul_0) { const char s[2] = { 0, 0 }; CHECK_EQ_I(mblen(s, 1), 0); }
TC_CASE(null_state_query_0) { CHECK_EQ_I(mblen(v_np, 0), 0); }
TC_CASE(dbcs_2)
{
    if (!setlocale(LC_ALL, ".932")) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    CHECK_EQ_I(mblen((const char *)hira, 2), 2);
    CHECK_EQ_I(mblen("a", 2), 1);
    setlocale(LC_ALL, "C");
}
TC_CASE(l_variant_dbcs_2)
{
    _locale_t L = tc_p__create_locale ? tc_p__create_locale(LC_ALL, ".932") : NULL;
    if (!L) TC_SKIP("no .932");
    CHECK_EQ_I(_mblen_l((const char *)hira, 2, L), 2);
    CHECK_EQ_I(_mblen_l("a", 2, L), 1);
    tc_p__free_locale(L);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_RUN(ascii_1); TC_RUN(nul_0); TC_RUN(null_state_query_0); TC_RUN(dbcs_2); TC_RUN(l_variant_dbcs_2); return tc_end(); }
