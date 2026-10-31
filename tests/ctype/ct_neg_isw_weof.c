/* TC
 * id:    ct_neg_isw_weof
 * area:  ctype
 * kind:  negative
 * funcs: towupper towlower
 * procs: iswalpha iswdigit iswspace iswpunct iswprint iswcntrl iswxdigit iswalnum iswgraph iswupper iswlower
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs iswalpha, towupper; WEOF == 0xFFFF
 * note:  U+FFFF is a noncharacter: no class, and the case maps leave it unchanged (WEOF). iswalpha(0) is a sanity case.
 */
#include "tc.h"
#include <wctype.h>

static volatile unsigned int vc;
typedef int (__cdecl *isw_t)(unsigned short);
static const char *const names[] = { "iswalpha", "iswdigit", "iswspace", "iswpunct", "iswprint", "iswcntrl", "iswxdigit", "iswalnum", "iswgraph", "iswupper", "iswlower" };

TC_CASE(all_isw_zero)
{
    size_t i; int n = 0;
    for (i = 0; i < sizeof names / sizeof names[0]; i++) {
        isw_t f = (isw_t)tc_crt_proc(names[i]);
        if (!f) { n |= 1 << 15; continue; }
        vc = 0xFFFF; if (f((unsigned short)vc)) n |= 1 << i;
    }
    CHECK_EQ_I(n, 0);
}
TC_CASE(towupper_WEOF_is_WEOF) { vc = 0xFFFF; CHECK_EQ_U(towupper((wint_t)vc), 0xFFFF); }
TC_CASE(towlower_WEOF_is_WEOF) { vc = 0xFFFF; CHECK_EQ_U(towlower((wint_t)vc), 0xFFFF); }
TC_CASE(iswalpha_0_zero) { isw_t f = (isw_t)tc_crt_proc("iswalpha"); CHECK(f != NULL); vc = 0; if (f) CHECK_EQ_I(f((unsigned short)vc), 0); }

int main(void) { tc_begin(); TC_RUN(all_isw_zero); TC_RUN(towupper_WEOF_is_WEOF); TC_RUN(towlower_WEOF_is_WEOF); TC_RUN(iswalpha_0_zero); return tc_end(); }
