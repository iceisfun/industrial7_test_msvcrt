/* TC
 * id:    scn_sscanf_l_basic
 * area:  scanf
 * funcs: _sscanf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sscanf, _sscanf_l; _create_locale
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static _locale_t L_;

TC_CASE(ints) { int a = 0, b = 0, r; r = _sscanf_l("12 -34", "%d %d", L_, &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, -34); }
TC_CASE(floats) { double d = 0; float f = 0; int r; r = _sscanf_l("1.5 2.25", "%lf %f", L_, &d, &f); CHECK_EQ_I(r, 2); CHECK_BITS_D(d, 1.5); CHECK_BITS_F(f, 2.25f); }
TC_CASE(floats_comma_not_radix) { double d = 0; int r; r = _sscanf_l("1,5", "%lf", L_, &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.0); }
TC_CASE(strings) { char a[16], b[16]; int r; a[0] = b[0] = 0; r = _sscanf_l("hello world", "%s %s", L_, a, b); CHECK_EQ_I(r, 2); CHECK_STR(a, "hello"); CHECK_STR(b, "world"); }
TC_CASE(percent_n) { int a = 0, n = -1, r; r = _sscanf_l("  42xy", "%d%n", L_, &a, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 42); CHECK_EQ_I(n, 4); }

int main(void)
{
    tc_begin();
    L_ = _create_locale(LC_ALL, "C");
    if (!L_) { tc_write("TC FAIL scn_sscanf_l_basic::create_locale\n"); return 1; }
    TC_RUN(ints); TC_RUN(floats); TC_RUN(floats_comma_not_radix); TC_RUN(strings); TC_RUN(percent_n);
    _free_locale(L_);
    return tc_end();
}
