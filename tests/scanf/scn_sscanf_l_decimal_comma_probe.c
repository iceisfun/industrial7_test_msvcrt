/* TC
 * id:    scn_sscanf_l_decimal_comma_probe
 * area:  scanf
 * funcs: _sscanf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _sscanf_l, _create_locale (locale-dependent decimal point)
 * note:  Probes that a German locale object makes %lf accept ',' as decimal point and reject '.', via _sscanf_l.
 *        Skipped when no German locale is installed. The C-locale case is documented/standard but kept in the
 *        same probe test only because it is the control for the same string.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static volatile const char *v;
TC_CASE(c_locale)
{
    double d = -1; char rest[8] = ""; int r; _locale_t loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL); if (!loc) return;
    v = "1,5";
    r = _sscanf_l((const char *)v, "%lf%7s", loc, &d, rest);
    CHECK_EQ_I(r, 2); CHECK_NEAR(d, 1.0, 0); CHECK_STR(rest, ",5");
    _free_locale(loc);
}
TC_CASE(german)
{
    double d = -1; char rest[8] = ""; int r;
    _locale_t loc = _create_locale(LC_ALL, "de-DE");
    if (!loc) loc = _create_locale(LC_ALL, "German_Germany");
    if (!loc) TC_SKIP("German locale not installed");
    v = "1,5";
    r = _sscanf_l((const char *)v, "%lf", loc, &d);
    CHECK_EQ_I(r, 1); CHECK_NEAR(d, 1.5, 1e-12);
    d = -1; rest[0] = 0; v = "1.5";
    r = _sscanf_l((const char *)v, "%lf%7s", loc, &d, rest);
    CHECK_EQ_I(r, 2); CHECK_NEAR(d, 1.0, 0); CHECK_STR(rest, ".5");
    _free_locale(loc);
}
int main(void) { tc_begin(); TC_RUN(c_locale); TC_RUN(german); return tc_end(); }
