/* TC
 * id:    ucr_strtol_bases
 * area:  ucrt_only
 * funcs: strtol strtoul
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.1.4 strtol/strtoul; MS docs strtol, strtoul
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_s;
static volatile int v_base;

static long L(const char *s, int base, int off)
{
    char *e = NULL; long r; v_s = s; v_base = base;
    r = strtol(v_s, &e, v_base);
    CHECK_EQ_I(e - s, off);
    return r;
}
TC_CASE(digits)
{
    CHECK_EQ_I(L("1234xyz", 10, 4), 1234);
    CHECK_EQ_I(L("777", 8, 3), 511);
    CHECK_EQ_I(L("ff", 16, 2), 255);
    CHECK_EQ_I(L("FfG", 16, 2), 255);
    CHECK_EQ_I(L("1011", 2, 4), 11);
    CHECK_EQ_I(L("zZ", 36, 2), 1295);
    CHECK_EQ_I(L("789", 8, 1), 7);
}
TC_CASE(base0_prefix)
{
    CHECK_EQ_I(L("0x1F", 0, 4), 31);
    CHECK_EQ_I(L("0X1f", 0, 4), 31);
    CHECK_EQ_I(L("017", 0, 3), 15);
    CHECK_EQ_I(L("99", 0, 2), 99);
    CHECK_EQ_I(L("0", 0, 1), 0);
}
TC_CASE(hex_prefix_base16) { CHECK_EQ_I(L("0x1f", 16, 4), 31); CHECK_EQ_I(L("0X10", 16, 4), 16); }
TC_CASE(space_sign)
{
    CHECK_EQ_I(L(" \t\n -42", 10, 7), -42);
    CHECK_EQ_I(L("+7", 10, 2), 7);
    CHECK_EQ_I(L("-0x10", 0, 5), -16);
}
TC_CASE(unsigned_variants)
{
    char *e = NULL; unsigned long r;
    v_s = "  0xFFFFFFFF"; v_base = 0;
    r = strtoul(v_s, &e, v_base);
    CHECK_EQ_U(r, 0xFFFFFFFFUL); CHECK_EQ_I(e - v_s, 12);
    v_s = "-1"; v_base = 10;
    r = strtoul(v_s, &e, v_base);   /* negation of 1 in unsigned arithmetic */
    CHECK_EQ_U(r, (unsigned long)-1L); CHECK_EQ_I(e - v_s, 2);
    v_s = "z"; v_base = 36;
    CHECK_EQ_U(strtoul(v_s, &e, v_base), 35);
}
TC_CASE(endptr_null)
{
    v_s = "42rest"; v_base = 10;
    CHECK_EQ_I(strtol(v_s, NULL, v_base), 42);
    CHECK_EQ_U(strtoul(v_s, NULL, v_base), 42);
}
int main(void)
{
    tc_begin();
    TC_RUN(digits); TC_RUN(base0_prefix); TC_RUN(hex_prefix_base16); TC_RUN(space_sign);
    TC_RUN(unsigned_variants); TC_RUN(endptr_null);
    return tc_end();
}
