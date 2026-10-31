/* TC
 * id:    dat_neg_pctype_c_locale_nonascii
 * area:  data_exports
 * funcs: isalpha isalnum isupper islower isgraph ispunct isprint
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs is* functions / setlocale (in the C locale only ASCII characters are classified)
 * note:  Bytes 128..255 and EOF must not be classified as anything in the C locale.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

TC_CASE(high_bytes_not_classified)
{
    int c, n = 0;
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
    for (c = 128; c < 256; c++)
        n += (isalpha(c) != 0) + (isalnum(c) != 0) + (isupper(c) != 0) + (islower(c) != 0) + (isgraph(c) != 0) + (ispunct(c) != 0) + (isprint(c) != 0);
    CHECK_EQ_I(n, 0);
}
TC_CASE(eof_not_classified)
{
    volatile int e = -1;
    CHECK(!isalpha(e)); CHECK(!isalnum(e)); CHECK(!isupper(e)); CHECK(!islower(e)); CHECK(!isgraph(e)); CHECK(!ispunct(e)); CHECK(!isprint(e));
}
TC_CASE(nul_not_printable) { volatile int c = 0; CHECK(!isprint(c)); CHECK(!isgraph(c)); CHECK(!isalnum(c)); }
int main(void) { tc_begin(); TC_RUN(high_bytes_not_classified); TC_RUN(eof_not_classified); TC_RUN(nul_not_printable); return tc_end(); }
