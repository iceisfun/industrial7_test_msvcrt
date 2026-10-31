/* TC
 * id:    mf_l_variants_inverse_trig
 * area:  math_float
 * funcs:
 * procs: asinl acosl atanl sinhl coshl tanhl asin acos atan sinh cosh tanh
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  inferred
 * ref:   MS docs "Long double" (long double == double); asin, acos, atan, sinh, cosh, tanh
 * note:  conf inferred: docs say long double == double; bit-identical results between the l-name and the double
 *        name assume the l-name is an alias/thunk of it (expected, not documented).
 *        Called through a double signature. A case skips if the l-name is not exported (some CRTs only have
 *        header inlines); presence is probed in mf_l_variants_exports_probe.
 */
#include "tc.h"

typedef double (__cdecl *d1_t)(double);
#define N 6
static d1_t p_l[N], p_d[N];
static const char *const lname[N] = { "asinl", "acosl", "atanl", "sinhl", "coshl", "tanhl" };
static const char *const dname[N] = { "asin", "acos", "atan", "sinh", "cosh", "tanh" };
static volatile double v_in[3] = { 0.25, -0.5, 0.9 };

static void run_pair(int k)
{
    int i;
    if (!p_d[k]) { CHECK(p_d[k] != 0); return; }
    if (!p_l[k]) TC_SKIP("l-variant not exported");
    for (i = 0; i < 3; i++) { volatile double x = v_in[i]; CHECK_BITS_D(p_l[k](x), p_d[k](x)); }
}
TC_CASE(asinl) { run_pair(0); }
TC_CASE(acosl) { run_pair(1); }
TC_CASE(atanl) { run_pair(2); }
TC_CASE(sinhl) { run_pair(3); }
TC_CASE(coshl) { run_pair(4); }
TC_CASE(tanhl) { run_pair(5); }

int main(void)
{
    int k; tc_begin();
    for (k = 0; k < N; k++) { p_l[k] = (d1_t)tc_crt_proc(lname[k]); p_d[k] = (d1_t)tc_crt_proc(dname[k]); }
    TC_RUN(asinl); TC_RUN(acosl); TC_RUN(atanl); TC_RUN(sinhl); TC_RUN(coshl); TC_RUN(tanhl);
    return tc_end();
}
