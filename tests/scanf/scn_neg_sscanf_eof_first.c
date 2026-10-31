/* TC
 * id:    scn_neg_sscanf_eof_first
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p16 (EOF if an input failure occurs before the first conversion)
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(empty_conversions)
{
    int d = 0; char s[4] = "", c = 0; float f = 0; char set[4] = "";
    vin = ""; CHECK_EQ_I(sscanf((const char *)vin, "%d", &d), EOF);
    vin = ""; CHECK_EQ_I(sscanf((const char *)vin, "%s", s), EOF);
    vin = ""; CHECK_EQ_I(sscanf((const char *)vin, "%c", &c), EOF);
    vin = ""; CHECK_EQ_I(sscanf((const char *)vin, "%f", &f), EOF);
    vin = ""; CHECK_EQ_I(sscanf((const char *)vin, "%[a]", set), EOF);
}
TC_CASE(whitespace_only)
{
    int d = 0; char s[4] = "";
    vin = "  \t\n"; CHECK_EQ_I(sscanf((const char *)vin, "%d", &d), EOF);
    vin = "  \t\n"; CHECK_EQ_I(sscanf((const char *)vin, "%s", s), EOF);
}
int main(void) { tc_begin(); TC_RUN(empty_conversions); TC_RUN(whitespace_only); return tc_end(); }
