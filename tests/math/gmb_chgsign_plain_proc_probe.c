/* TC
 * id:    gmb_chgsign_plain_proc_probe
 * area:  math
 * funcs: 
 * procs: chgsign _chgsignf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _chgsign
 * note:  PROBE: availability of plain chgsign and float _chgsignf (64-bit CRTs only); absent => SKIP.
 */
#include "tc.h"
#include <float.h>
#include <math.h>

typedef double (__cdecl *d_d)(double);
typedef float (__cdecl *f_f)(float);

TC_CASE(plain) { d_d p = (d_d)tc_crt_proc("chgsign"); volatile double x = 2.5; if (!p) TC_SKIP("chgsign not exported"); CHECK_BITS_D(p(x), -2.5); }
TC_CASE(float_version) { f_f p = (f_f)tc_crt_proc("_chgsignf"); volatile float x = 2.5f; if (!p) TC_SKIP("_chgsignf not exported"); CHECK_BITS_F(p(x), -2.5f); }
int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(float_version); return tc_end(); }
