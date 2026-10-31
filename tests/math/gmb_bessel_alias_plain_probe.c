/* TC
 * id:    gmb_bessel_alias_plain_probe
 * area:  math
 * funcs:
 * procs: j0 j1 jn y0 y1 yn
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _j0 _y0 (underscore names are the documented ones)
 * note:  PROBE: whether the POSIX-name aliases are real DLL exports (GetProcAddress). Absence is a SKIP, never a failure. If present they must match the underscore versions.
 */
#include "tc.h"
#include <math.h>
#include <float.h>

typedef double (__cdecl *d_d)(double);
typedef double (__cdecl *d_id)(int, double);
typedef int (__cdecl *i_d)(double);

static void alias_d(const char *nm, double (__cdecl *under)(double), double arg, int *present)
{
    d_d p = (d_d)tc_crt_proc(nm); *present = p != NULL;
    if (p) CHECK_BITS_D(p(arg), under(arg));
}
static void alias_id(const char *nm, double (__cdecl *under)(int, double), int *present)
{
    d_id p = (d_id)tc_crt_proc(nm); *present = p != NULL;
    if (p) CHECK_BITS_D(p(2, 1.0), under(2, 1.0));
}
TC_CASE(j0) { int pr; alias_d("j0", _j0, 1.0, &pr); if (!pr) TC_SKIP("j0 not exported"); }
TC_CASE(j1) { int pr; alias_d("j1", _j1, 1.0, &pr); if (!pr) TC_SKIP("j1 not exported"); }
TC_CASE(jn) { int pr; alias_id("jn", _jn, &pr); if (!pr) TC_SKIP("jn not exported"); }
TC_CASE(y0) { int pr; alias_d("y0", _y0, 1.0, &pr); if (!pr) TC_SKIP("y0 not exported"); }
TC_CASE(y1) { int pr; alias_d("y1", _y1, 1.0, &pr); if (!pr) TC_SKIP("y1 not exported"); }
TC_CASE(yn) { int pr; alias_id("yn", _yn, &pr); if (!pr) TC_SKIP("yn not exported"); }
int main(void)
{
    tc_begin(); TC_RUN(j0); TC_RUN(j1); TC_RUN(jn); TC_RUN(y0); TC_RUN(y1); TC_RUN(yn);
   
    return tc_end();
}
