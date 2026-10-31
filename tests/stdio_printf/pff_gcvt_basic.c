/* TC
 * id:    pff_gcvt_basic
 * area:  stdio_printf
 * funcs: _gcvt
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gcvt
 * note:  the trailing '.' for integral values ("100.") is MS-specific, so only the "100" prefix is asserted. Exponent digit count (2 vs 3) differs between CRT generations, so either is accepted.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

TC_CASE(simple)  { char b[32]; volatile double v = 1.5; char *r = _gcvt(v, 6, b); CHECK_STR(b, "1.5"); CHECK_EQ_P(r, b); }
TC_CASE(small)   { char b[32]; volatile double v = 0.0001234; _gcvt(v, 5, b); CHECK_STR(b, "0.0001234"); }
TC_CASE(integral){ char b[32]; volatile double v = 100.0; _gcvt(v, 3, b); CHECK(strncmp(b, "100", 3) == 0); CHECK(b[3] == 0 || (b[3] == '.' && b[4] == 0)); }
TC_CASE(rounded) { char b[32]; volatile double v = 1234.5678; _gcvt(v, 6, b); CHECK_STR(b, "1234.57"); }
TC_CASE(exponent)
{
    char b[32]; volatile double v = 1.5e10; _gcvt(v, 5, b);
    CHECK(strncmp(b, "1.5e+", 5) == 0);
    CHECK(strcmp(b + 5, "10") == 0 || strcmp(b + 5, "010") == 0);
}
int main(void) { tc_begin(); TC_RUN(simple); TC_RUN(small); TC_RUN(integral); TC_RUN(rounded); TC_RUN(exponent); return tc_end(); }
