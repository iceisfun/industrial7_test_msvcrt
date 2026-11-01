/* TC
 * id:    pff_fcvt_s_exact_buf_probe
 * area:  stdio_printf
 * funcs:
 * procs: _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _fcvt_s: sizeInBytes
 * note:  OBSERVED on ucrtbase only (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "fcvt_s size n=4 r=34 d=-99 s=-99 b=[]", "n=5 r=0 d=1 s=0 b=[314]"):
 *        _fcvt_s(buf, 4, 3.14159, 2) is rejected with ERANGE and buf[0]=0 although "314"+NUL fits in 4 bytes; the converter needs one byte more
 *        (5 -> "314", decpt 1, sign 0). The original belief (exact fit accepted) is kept for pre-UCRT CRTs (#else, unverified). Not a C
 *        function; no ISO text. Stable: deterministic, no locale/time dependence.
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
TC_CASE(exact) { char b[4]; int d = -99, s = -99, r; volatile double v = 3.14159; if (!tc_p__fcvt_s) TC_SKIP("none");
#if TC_CRT_VER >= 140
  b[0] = 'x'; r = tc_p__fcvt_s(b, 4, v, 2, &d, &s); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 0);   /* ucrtbase (observed) */
#else
  r = tc_p__fcvt_s(b, 4, v, 2, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "314");   /* original expectation, pre-UCRT CRTs (not verified on the reference) */
#endif
}
TC_CASE(one_extra) { char b[5]; int d = -99, s = -99, r; volatile double v = 3.14159; if (!tc_p__fcvt_s) TC_SKIP("none");
  r = tc_p__fcvt_s(b, 5, v, 2, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "314"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(_fcvt_s); CHECK(tc_p__fcvt_s != NULL); TC_RUN(exact); TC_RUN(one_extra); return tc_end(); }
