/* TC
 * id:    cv_gcvt_strtod_denorm_probe
 * area:  conv
 * funcs: _gcvt
 * procs: strtod _gcvt_s
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.22.1.3; MS docs _gcvt, strtod
 * note:  Probes that the smallest denormal (4.9406564584124654e-324) round-trips through 17-digit _gcvt/_gcvt_s and strtod.
 *        Expected on UCRT; pre-UCRT denormal handling in strtod is undocumented.
 */
#include "tc.h"

TC_DECL_PROC(double, strtod, (const char *, char **));
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
extern char *__cdecl _gcvt(double, int, char *);

TC_CASE(literal)
{
    volatile double dn = 4.9406564584124654e-324;
    CHECK_BITS_D(tc_p_strtod("4.9406564584124654e-324", NULL), dn);
}
TC_CASE(gcvt_roundtrip)
{
    char b[64]; volatile double dn = 4.9406564584124654e-324;
    _gcvt(dn, 17, b);
    CHECK_BITS_D(tc_p_strtod(b, NULL), dn);
}
TC_CASE(gcvt_s_roundtrip)
{
    char b[64]; volatile double dn = 4.9406564584124654e-324;
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, dn, 17), 0);
    CHECK_BITS_D(tc_p_strtod(b, NULL), dn);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(strtod); TC_LOAD_PROC(_gcvt_s);
    if (!tc_p_strtod) { tc_write("TC FAIL setup\n"); return 1; }
    TC_RUN(literal); TC_RUN(gcvt_roundtrip); TC_RUN(gcvt_s_roundtrip);
    return tc_end();
}
