/* TC
 * id:    gmb_finite_fpclass_alias_probe
 * area:  math
 * funcs:
 * procs: chgsign finite fpclass
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _chgsign _finite _fpclass
 * note:  PROBE: whether the plain names chgsign/finite/fpclass are real DLL exports; absent => SKIP; present => same results as underscore versions.
 */
#include "tc.h"
#include <float.h>
#include <math.h>

typedef double (__cdecl *d_d)(double);
typedef int (__cdecl *i_d)(double);

TC_CASE(chgsign) { d_d p = (d_d)tc_crt_proc("chgsign"); volatile double x = 1.5; if (!p) TC_SKIP("chgsign not exported"); CHECK_BITS_D(p(x), _chgsign(x)); }
TC_CASE(finite)
{
    i_d p = (i_d)tc_crt_proc("finite"); volatile double x = 1.0, z = 0.0, inf = 1.0 / z;
    if (!p) TC_SKIP("finite not exported");
    CHECK(p(x) != 0); CHECK(p(inf) == 0);
}
TC_CASE(fpclass)
{
    i_d p = (i_d)tc_crt_proc("fpclass"); volatile double x = 1.0, z = 0.0;
    if (!p) TC_SKIP("fpclass not exported");
    CHECK_EQ_I(p(x), 0x100); CHECK_EQ_I(p(z), 0x40);
}
int main(void) { tc_begin(); TC_RUN(chgsign); TC_RUN(finite); TC_RUN(fpclass); return tc_end(); }
