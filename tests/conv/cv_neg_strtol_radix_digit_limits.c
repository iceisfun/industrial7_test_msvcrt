/* TC
 * id:    cv_neg_strtol_radix_digit_limits
 * area:  conv
 * funcs: strtol strtoul wcstol
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4 (subject sequence of letters/digits whose value is below base); MS docs strtol
 * note:  strtoll omitted (absent from older CRTs). Fullwidth digits are in cv_neg_wcstol_fullwidth_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

static const char *volatile vs;
static const wchar_t *volatile vw;

TC_CASE(octal_stops_at_8) { char *e; vs = "789"; CHECK_EQ_I(strtol(vs, &e, 8), 7); CHECK_EQ_P(e, vs + 1); }
TC_CASE(octal_prefix_digits) { char *e; vs = "789"; vs = "0789"; CHECK_EQ_I(strtol(vs, &e, 0), 7); CHECK_EQ_P(e, vs + 2); }
TC_CASE(binary_2) { char *e; vs = "2"; CHECK_EQ_I(strtol(vs, &e, 2), 0); CHECK_EQ_P(e, vs); vs = "1012"; CHECK_EQ_I(strtol(vs, &e, 2), 5); CHECK_EQ_P(e, vs + 3); }
TC_CASE(base36)
{
    char *e;
    vs = "z"; CHECK_EQ_I(strtol(vs, &e, 36), 35); CHECK_EQ_P(e, vs + 1);
    vs = "Z"; CHECK_EQ_I(strtol(vs, &e, 35), 0);  CHECK_EQ_P(e, vs);
    vs = "Zz"; CHECK_EQ_I(strtol(vs, &e, 36), 35 * 36 + 35);
}
TC_CASE(no_0b_prefix) { char *e; vs = "0b101"; CHECK_EQ_I(strtol(vs, &e, 2), 0); CHECK_EQ_P(e, vs + 1); }
TC_CASE(hex_g) { char *e; vs = "0x1g"; CHECK_EQ_I(strtol(vs, &e, 16), 1); CHECK_EQ_P(e, vs + 3); }
TC_CASE(unsigned_and_wide)
{
    char *e; wchar_t *we;
    vs = "19"; CHECK_EQ_U(strtoul(vs, &e, 8), 1); CHECK_EQ_P(e, vs + 1);
    vw = L"789"; CHECK_EQ_I(wcstol(vw, &we, 8), 7); CHECK_EQ_P(we, vw + 1);
    vw = L"12z"; CHECK_EQ_I(wcstol(vw, &we, 10), 12); CHECK_EQ_P(we, vw + 2);
}
int main(void)
{
    tc_begin();
    TC_RUN(octal_stops_at_8); TC_RUN(octal_prefix_digits); TC_RUN(binary_2); TC_RUN(base36); TC_RUN(no_0b_prefix);
    TC_RUN(hex_g); TC_RUN(unsigned_and_wide);
    return tc_end();
}
