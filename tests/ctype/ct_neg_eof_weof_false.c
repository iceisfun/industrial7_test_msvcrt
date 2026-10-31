/* TC
 * id:    ct_neg_eof_weof_false
 * area:  ctype
 * funcs: isalpha isdigit isspace isupper toupper tolower iswalpha iswdigit iswspace towupper towlower
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs isalpha: EOF is valid input; C11 7.4, 7.30.1: classification of EOF/WEOF is false
 * note:  EOF (-1) and WEOF (0xFFFF) are classified as nothing and case-mapped to themselves.
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>
#include <stdio.h>
#undef isalpha
#undef isdigit
#undef isspace
#undef isupper
#undef toupper
#undef tolower
#undef iswalpha
#undef iswdigit
#undef iswspace
#undef towupper
#undef towlower

static volatile int e = EOF;
static volatile wint_t we = WEOF;

TC_CASE(narrow_eof_false)
{
    CHECK_EQ_I(isalpha(e), 0); CHECK_EQ_I(isdigit(e), 0); CHECK_EQ_I(isspace(e), 0); CHECK_EQ_I(isupper(e), 0);
}
TC_CASE(narrow_eof_case_unchanged) { CHECK_EQ_I(toupper(e), EOF); CHECK_EQ_I(tolower(e), EOF); }
TC_CASE(wide_weof_false)
{
    CHECK_EQ_I(iswalpha(we), 0); CHECK_EQ_I(iswdigit(we), 0); CHECK_EQ_I(iswspace(we), 0);
}
TC_CASE(wide_weof_case_unchanged) { CHECK_EQ_U(towupper(we), WEOF); CHECK_EQ_U(towlower(we), WEOF); }
int main(void)
{
    tc_begin();
    TC_RUN(narrow_eof_false); TC_RUN(narrow_eof_case_unchanged); TC_RUN(wide_weof_false); TC_RUN(wide_weof_case_unchanged);
    return tc_end();
}
