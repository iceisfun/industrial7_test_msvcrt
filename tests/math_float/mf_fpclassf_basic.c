/* TC
 * id:    mf_fpclassf_basic
 * area:  math_float
 * funcs:
 * procs: _fpclassf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fpclass, _fpclassf (float.h _FPCLASS_* values)
 * note:  signalling NaN is in mf_fpclassf_snan_probe.
 */
#include "tc.h"
#include <float.h>
/* F: a direct call of this name linked/called with the wrong signature on mingw (baseline results were 0 for every input); the DLL export called through GetProcAddress with the real float prototype returns the documented values */
TC_DECL_PROC(int, _fpclassf, (float));
#undef _fpclassf
#define _fpclassf tc_p__fpclassf
static float mk(unsigned int b) { union { unsigned int u; float f; } x; x.u = b; return x.f; }
static volatile unsigned int b_pz = 0, b_nz = 0x80000000u, b_pn = 0x3F800000u, b_nn = 0xBF800000u, b_pd = 0x00000001u, b_nd = 0x80000001u,
    b_pinf = 0x7F800000u, b_ninf = 0xFF800000u, b_qnan = 0x7FC00000u;

TC_CASE(zeros) { CHECK_EQ_I(_fpclassf(mk(b_pz)), 0x40); CHECK_EQ_I(_fpclassf(mk(b_nz)), 0x20); }
TC_CASE(normals) { CHECK_EQ_I(_fpclassf(mk(b_pn)), 0x100); CHECK_EQ_I(_fpclassf(mk(b_nn)), 0x08); }
TC_CASE(denormals) { CHECK_EQ_I(_fpclassf(mk(b_pd)), 0x80); CHECK_EQ_I(_fpclassf(mk(b_nd)), 0x10); }
TC_CASE(infinities) { CHECK_EQ_I(_fpclassf(mk(b_pinf)), 0x200); CHECK_EQ_I(_fpclassf(mk(b_ninf)), 0x04); }
TC_CASE(qnan) { CHECK_EQ_I(_fpclassf(mk(b_qnan)), 0x02); }
int main(void) { tc_begin(); TC_LOAD_PROC(_fpclassf); if (!tc_p__fpclassf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(zeros); TC_RUN(normals); TC_RUN(denormals); TC_RUN(infinities); TC_RUN(qnan); return tc_end(); }
