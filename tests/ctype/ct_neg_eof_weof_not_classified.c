/* TC
 * id:    ct_neg_eof_weof_not_classified
 * area:  ctype
 * funcs: isalpha isdigit isspace ispunct iscntrl isxdigit iswalpha iswdigit iswspace iswpunct iswcntrl iswxdigit _isctype iswctype
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.4p1 (EOF is a valid argument), 7.30.2.1 (WEOF is a valid argument); MS docs isalpha: EOF is allowed
 * note:  isw* and is* on EOF / WEOF are false.
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>
#include <stdio.h>

TC_CASE(eof_is_not_classified)
{
    volatile int e = EOF;
    CHECK(!isalpha(e)); CHECK(!isdigit(e)); CHECK(!isspace(e)); CHECK(!ispunct(e)); CHECK(!iscntrl(e)); CHECK(!isxdigit(e));
}
TC_CASE(eof_isctype_all_masks)
{
    volatile int e = EOF;
    CHECK(!_isctype(e, _ALPHA)); CHECK(!_isctype(e, _UPPER|_LOWER|_DIGIT|_SPACE|_PUNCT|_CONTROL|_HEX));
}
TC_CASE(weof_is_not_classified)
{
    volatile wint_t e = WEOF;
    CHECK(!(iswalpha)(e)); CHECK(!(iswdigit)(e)); CHECK(!(iswspace)(e)); CHECK(!(iswpunct)(e)); CHECK(!(iswcntrl)(e)); CHECK(!(iswxdigit)(e));
}
TC_CASE(weof_iswctype)
{
    volatile wint_t e = WEOF;
    CHECK(!iswctype(e, _ALPHA)); CHECK(!iswctype(e, _DIGIT)); CHECK(!iswctype(e, _SPACE));
}
int main(void) { tc_begin(); TC_RUN(eof_is_not_classified); TC_RUN(eof_isctype_all_masks); TC_RUN(weof_is_not_classified); TC_RUN(weof_iswctype); return tc_end(); }
