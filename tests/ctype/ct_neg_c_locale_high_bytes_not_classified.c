/* TC
 * id:    ct_neg_c_locale_high_bytes_not_classified
 * area:  ctype
 * funcs: isalpha isalnum isupper islower ispunct isprint isgraph iscntrl toupper tolower
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.4.1.2/7.4.1.9/7.4.1.11 (alpha/lower/upper in C locale: ASCII letters only); MS C-locale ctype table is zero above 0x7F
 * note:  In the C locale, bytes 0x80-0xFF are never alpha/upper/lower/punct/print/graph/cntrl and case-map to themselves; ASCII neighbours are still classified.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static volatile int c;

TC_CASE(high_bytes_unclassified)
{
    int i;
    setlocale(LC_ALL, "C");
    for (i = 0x80; i <= 0xFF; i++) {
        c = i;
        if (isalpha(c) || isalnum(c) || isupper(c) || islower(c) || ispunct(c) || isprint(c) || isgraph(c) || iscntrl(c))
            { CHECK_EQ_I(i, -1); break; }
    }
}
TC_CASE(high_bytes_case_unchanged)
{
    int i;
    setlocale(LC_ALL, "C");
    for (i = 0x80; i <= 0xFF; i++) {
        c = i;
        if (toupper(c) != i || tolower(c) != i) { CHECK_EQ_I(i, -1); break; }
    }
}
TC_CASE(ascii_boundaries)
{
    c = '@'; CHECK_EQ_I(isalpha(c), 0); c = '['; CHECK_EQ_I(isalpha(c), 0);
    c = '`'; CHECK_EQ_I(isalpha(c), 0); c = '{'; CHECK_EQ_I(isalpha(c), 0);
    c = '/'; CHECK_EQ_I(isdigit(c), 0); c = ':'; CHECK_EQ_I(isdigit(c), 0);
    c = 'G'; CHECK_EQ_I(isxdigit(c), 0); c = 0x7F; CHECK(iscntrl(c) != 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(high_bytes_unclassified); TC_RUN(high_bytes_case_unchanged); TC_RUN(ascii_boundaries);
    return tc_end();
}
