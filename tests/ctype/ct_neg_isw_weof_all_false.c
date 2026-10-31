/* TC
 * id:    ct_neg_isw_weof_all_false
 * area:  ctype
 * funcs: iswalpha iswdigit iswspace iswpunct iswupper iswlower iswcntrl iswprint iswgraph iswalnum iswxdigit towupper towlower
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.30.1 (WEOF valid, belongs to no class); MS docs iswalpha
 * note:  WEOF is 0xFFFF.
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

static volatile wint_t v = WEOF;

TC_CASE(weof_in_no_class)
{
    CHECK_EQ_I((iswalpha)(v), 0); CHECK_EQ_I((iswdigit)(v), 0); CHECK_EQ_I((iswspace)(v), 0);
    CHECK_EQ_I((iswpunct)(v), 0); CHECK_EQ_I((iswupper)(v), 0); CHECK_EQ_I((iswlower)(v), 0);
    CHECK_EQ_I((iswcntrl)(v), 0); CHECK_EQ_I((iswprint)(v), 0); CHECK_EQ_I((iswgraph)(v), 0);
    CHECK_EQ_I((iswalnum)(v), 0); CHECK_EQ_I((iswxdigit)(v), 0);
}
TC_CASE(weof_case_conversion_identity) { CHECK_EQ_U((towupper)(v), WEOF); CHECK_EQ_U((towlower)(v), WEOF); }

int main(void) { tc_begin(); TC_RUN(weof_in_no_class); TC_RUN(weof_case_conversion_identity); return tc_end(); }
