/* TC
 * id:    pff_fcvt_s_exact_buf_probe
 * area:  stdio_printf
 * funcs:
 * procs: _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _fcvt_s: sizeInBytes
 * note:  probed: buffer of exactly strlen(digits)+1 bytes is accepted (rc 0). Some CRT versions may demand more room.
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
TC_CASE(exact) { char b[4]; int d = -99, s = -99, r; volatile double v = 3.14159; if (!tc_p__fcvt_s) TC_SKIP("none");
  r = tc_p__fcvt_s(b, 4, v, 2, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "314"); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(_fcvt_s); CHECK(tc_p__fcvt_s != NULL); TC_RUN(exact); return tc_end(); }
