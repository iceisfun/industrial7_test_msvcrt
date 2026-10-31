/* TC
 * id:    cv_neg_strtol_wrong_forms
 * area:  conv
 * funcs: strtol strtoul _strtoi64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4: no valid subject sequence -> 0, *endptr = nptr; otherwise endptr after longest valid prefix
 * note:  Non-ASCII digits (Devanagari, Arabic-Indic) are multibyte sequences in UTF-8 and must never convert in the C locale.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile vs;

static void none(const char *s)
{
    char *e = (char *)1; vs = s;
    CHECK_EQ_I(strtol((const char *)vs, &e, 10), 0); CHECK(e == s);
    e = (char *)1; CHECK_EQ_U(strtoul((const char *)vs, &e, 10), 0); CHECK(e == s);
    e = (char *)1; CHECK_EQ_I(_strtoi64((const char *)vs, &e, 10), 0); CHECK(e == s);
}
static void part(const char *s, long want, int used)
{
    char *e = (char *)1; vs = s;
    CHECK_EQ_I(strtol((const char *)vs, &e, 10), want); CHECK(e == s + used);
    e = (char *)1; CHECK_EQ_U(strtoul((const char *)vs, &e, 10), (unsigned long)want); CHECK(e == s + used);
    e = (char *)1; CHECK_EQ_I(_strtoi64((const char *)vs, &e, 10), want); CHECK(e == s + used);
}
TC_CASE(double_sign) { none("+-5"); none("--5"); none("++5"); none("-+5"); }
TC_CASE(sign_only)   { none("-"); none("+"); none(""); none("   "); }
TC_CASE(hex_prefix_base10) { part("0x", 0, 1); part("0x10", 0, 1); }
TC_CASE(stops)       { part("1_000", 1, 1); part("1 2", 1, 1); part("12.5", 12, 2); part("7abc", 7, 1); }
TC_CASE(non_ascii_digits) { none("\xe0\xa5\xa7\xe0\xa5\xa8\xe0\xa5\xa9"); none("\xd9\xa3"); part("5\xd9\xa3", 5, 1); }

int main(void)
{
    tc_begin();
    TC_RUN(double_sign); TC_RUN(sign_only); TC_RUN(hex_prefix_base10); TC_RUN(stops); TC_RUN(non_ascii_digits);
    return tc_end();
}
