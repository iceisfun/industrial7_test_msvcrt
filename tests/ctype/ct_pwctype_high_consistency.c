/* TC
 * id:    ct_pwctype_high_consistency
 * area:  ctype
 * funcs: iswalpha iswdigit iswspace iswpunct iswcntrl iswxdigit iswupper iswlower
 * procs: __pwctype_func
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs iswctype
 * note:  PROBE: for c=128..255 in the C locale the isw* result equals the __pwctype_func table bit (self-consistency only, no absolute value).
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

TC_CASE(high_consistent)
{
    const unsigned short *(__cdecl *pw)(void) = (const unsigned short *(__cdecl *)(void))tc_crt_proc("__pwctype_func");
    const unsigned short *t = pw ? pw() : NULL;
    volatile int i;
    if (!t) TC_SKIP("no table");
    for (i = 128; i < 256; i++) {
        wint_t c = (wint_t)i;
        CHECK_EQ_I(((iswalpha)(c) != 0), ((t[i] & _ALPHA) != 0));
        CHECK_EQ_I(((iswdigit)(c) != 0), ((t[i] & _DIGIT) != 0));
        CHECK_EQ_I(((iswspace)(c) != 0), ((t[i] & _SPACE) != 0));
        CHECK_EQ_I(((iswpunct)(c) != 0), ((t[i] & _PUNCT) != 0));
        CHECK_EQ_I(((iswcntrl)(c) != 0), ((t[i] & _CONTROL) != 0));
        CHECK_EQ_I(((iswxdigit)(c) != 0), ((t[i] & _HEX) != 0));
        CHECK_EQ_I(((iswupper)(c) != 0), ((t[i] & _UPPER) != 0));
        CHECK_EQ_I(((iswlower)(c) != 0), ((t[i] & _LOWER) != 0));
    }
}
int main(void) { tc_begin(); TC_RUN(high_consistent); return tc_end(); }
