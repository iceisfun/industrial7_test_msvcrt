/* TC
 * id:    ct_pwctype_table_consistency
 * area:  ctype
 * funcs: iswalpha iswdigit iswspace iswpunct iswcntrl iswxdigit iswupper iswlower
 * procs: __pwctype_func
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs iswctype; ctype.h masks
 * note:  Only 0..127 asserted against the table bits.
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

static const unsigned short *(__cdecl *p_pw)(void);
static const unsigned short *tab(void) { return p_pw ? p_pw() : NULL; }
#define BITS(c) (tab()[(c)])

TC_CASE(pwctype_non_null) { CHECK(tab() != NULL); }
TC_CASE(ascii_bits_match_isw)
{
    volatile int i;
    if (!tab()) TC_SKIP("no table");
    for (i = 0; i < 128; i++) {
        wint_t c = (wint_t)i;
        CHECK_EQ_I(((iswalpha)(c) != 0), ((BITS(i) & _ALPHA) != 0));
        CHECK_EQ_I(((iswdigit)(c) != 0), ((BITS(i) & _DIGIT) != 0));
        CHECK_EQ_I(((iswspace)(c) != 0), ((BITS(i) & _SPACE) != 0));
        CHECK_EQ_I(((iswpunct)(c) != 0), ((BITS(i) & _PUNCT) != 0));
        CHECK_EQ_I(((iswcntrl)(c) != 0), ((BITS(i) & _CONTROL) != 0));
        CHECK_EQ_I(((iswxdigit)(c) != 0), ((BITS(i) & _HEX) != 0));
        CHECK_EQ_I(((iswupper)(c) != 0), ((BITS(i) & _UPPER) != 0));
        CHECK_EQ_I(((iswlower)(c) != 0), ((BITS(i) & _LOWER) != 0));
    }
}
TC_CASE(digit_bit_for_0_9)
{
    volatile int i;
    if (!tab()) TC_SKIP("no table");
    for (i = '0'; i <= '9'; i++) CHECK(BITS(i) & _DIGIT);
    CHECK(!(BITS('a') & _DIGIT));
    CHECK(!(BITS('/') & _DIGIT));
    CHECK(!(BITS(':') & _DIGIT));
}
TC_CASE(upper_lower_bits_for_az)
{
    volatile int i;
    if (!tab()) TC_SKIP("no table");
    for (i = 'A'; i <= 'Z'; i++) { CHECK(BITS(i) & _UPPER); CHECK(!(BITS(i) & _LOWER)); }
    for (i = 'a'; i <= 'z'; i++) { CHECK(BITS(i) & _LOWER); CHECK(!(BITS(i) & _UPPER)); }
}
int main(void) { tc_begin(); p_pw = (const unsigned short *(__cdecl *)(void))tc_crt_proc("__pwctype_func"); TC_RUN(pwctype_non_null); TC_RUN(ascii_bits_match_isw); TC_RUN(digit_bit_for_0_9); TC_RUN(upper_lower_bits_for_az); return tc_end(); }
