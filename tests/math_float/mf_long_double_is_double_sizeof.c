/* TC
 * id:    mf_long_double_is_double_sizeof
 * area:  math_float
 * funcs:
 * procs: sinl cosl tanl sin cos tan
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs "Long double" / sinl, cosl, tanl: long double is the same as double on MSVC
 * note:  conf inferred: docs say long double == double; bit-identical results between the l-name and the double
 *        name assume the l-name is an alias/thunk of it (expected, not documented).
 *        The l-names are called through a double signature (MSVC ABI: long double == double), not through the
 *        mingw 16-byte long double ABI. A case skips if the l-name is not exported by that CRT (several CRTs
 *        provide these only as header inlines); export presence is probed in mf_l_variants_exports_probe.
 */
#include "tc.h"

typedef double (__cdecl *d1_t)(double);
static d1_t p_l[3], p_d[3];
static const char *const lname[3] = { "sinl", "cosl", "tanl" };
static const char *const dname[3] = { "sin", "cos", "tan" };
static volatile double v_in[6] = { 0.0, 0.5, 1.0, -2.0, 3.14159, 100.0 };

static void run_pair(int k)
{
    int i;
    if (!p_d[k]) { CHECK(p_d[k] != 0); return; }
    if (!p_l[k]) TC_SKIP("l-variant not exported");
    for (i = 0; i < 6; i++) { volatile double x = v_in[i]; CHECK_BITS_D(p_l[k](x), p_d[k](x)); }
}
TC_CASE(sinl_eq_sin) { run_pair(0); }
TC_CASE(cosl_eq_cos) { run_pair(1); }
TC_CASE(tanl_eq_tan) { run_pair(2); }
TC_CASE(sinl_zero) { if (!p_l[0]) TC_SKIP("sinl not exported"); CHECK_BITS_D(p_l[0](0.0), 0.0); CHECK_BITS_D(p_l[0](-0.0), -0.0); }

int main(void)
{
    int k; tc_begin();
    for (k = 0; k < 3; k++) { p_l[k] = (d1_t)tc_crt_proc(lname[k]); p_d[k] = (d1_t)tc_crt_proc(dname[k]); }
    TC_RUN(sinl_eq_sin); TC_RUN(cosl_eq_cos); TC_RUN(tanl_eq_tan); TC_RUN(sinl_zero);
    return tc_end();
}
