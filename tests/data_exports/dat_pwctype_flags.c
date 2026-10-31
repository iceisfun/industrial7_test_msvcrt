/* TC
 * id:    dat_pwctype_flags
 * area:  data_exports
 * funcs:
 * procs: __pwctype_func iswalpha iswdigit
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   ctype.h flag values (_UPPER 1 _DIGIT 4 _SPACE 8 _ALPHA 0x100); MS docs iswalpha/iswdigit
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

typedef const unsigned short *(__cdecl *tabfn_t)(void);
typedef int (__cdecl *isw_t)(wint_t);
static tabfn_t p_pwctype_func; static isw_t p_iswalpha, p_iswdigit;

TC_CASE(pwctype_A_alpha) { const unsigned short *t = p_pwctype_func(); volatile int c = 'A'; CHECK(t[c] & 0x103); CHECK(t[c] & 1); }
TC_CASE(pwctype_5_digit) { const unsigned short *t = p_pwctype_func(); volatile int c = '5'; CHECK(t[c] & 4); CHECK(!(t[c] & 0x103)); }
TC_CASE(pwctype_space)   { const unsigned short *t = p_pwctype_func(); volatile int c = ' '; CHECK(t[c] & 8); CHECK(!(t[c] & 0x103)); }
TC_CASE(table_agrees_with_iswalpha_ascii)
{
    const unsigned short *t = p_pwctype_func(); int c, n = 0;
    for (c = 0; c < 128; c++) {
        if ((p_iswalpha((wint_t)c) != 0) != ((t[c] & 0x103) != 0)) n++;
        if ((p_iswdigit((wint_t)c) != 0) != ((t[c] & 4) != 0)) n++;
    }
    CHECK_EQ_I(n, 0);
}
int main(void) { tc_begin(); p_pwctype_func = (tabfn_t)tc_crt_proc("__pwctype_func"); p_iswalpha = (isw_t)tc_crt_proc("iswalpha"); p_iswdigit = (isw_t)tc_crt_proc("iswdigit"); if (!p_pwctype_func || !p_iswalpha || !p_iswdigit) { tc_write("TC FAIL dat_pwctype_flags::export missing\n"); return 1; } TC_RUN(pwctype_A_alpha); TC_RUN(pwctype_5_digit); TC_RUN(pwctype_space); TC_RUN(table_agrees_with_iswalpha_ascii); return tc_end(); }
