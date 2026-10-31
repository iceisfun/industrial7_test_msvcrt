/* TC
 * id:    cv_strtol_utf_locale_free
 * area:  conv
 * funcs: strtol
 * crt:   all
 * tier:  p1
 * conf:  inferred
 * ref:   C11 7.22.1.4; C11 7.4.1.10 isspace in the "C" locale is only space,\t\n\v\f\r
 * note:  Tests start in the C locale, where bytes >= 0x80 are neither spaces nor digits. The negative-char case guards against isspace(negative char) table underflow.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile vs;

TC_CASE(high_byte_terminates)
{
    char *e; vs = "12\xff";
    CHECK_EQ_I(strtol((const char *)vs, &e, 10), 12); CHECK(e == vs + 2);
}
TC_CASE(nbsp_not_space)
{
    char *e; vs = "\xa0" "12";
    CHECK_EQ_I(strtol((const char *)vs, &e, 10), 0); CHECK(e == vs);
}
TC_CASE(embedded_nul)
{
    char *e; static const char s[] = "12\0 34"; vs = s;
    CHECK_EQ_I(strtol((const char *)vs, &e, 10), 12); CHECK(e == vs + 2);
}
TC_CASE(negative_char_leading)
{
    char *e; vs = "\xe9" "5";
    CHECK_EQ_I(strtol((const char *)vs, &e, 10), 0); CHECK(e == vs);
    vs = "\x80\x81\xff";
    CHECK_EQ_I(strtol((const char *)vs, &e, 16), 0); CHECK(e == vs);
}
TC_CASE(space_then_high)
{
    char *e; vs = "  -7\xc3\xa9";
    CHECK_EQ_I(strtol((const char *)vs, &e, 10), -7); CHECK(e == vs + 4);
}
int main(void)
{
    tc_begin();
    TC_RUN(high_byte_terminates); TC_RUN(nbsp_not_space); TC_RUN(embedded_nul); TC_RUN(negative_char_leading); TC_RUN(space_then_high);
    return tc_end();
}
