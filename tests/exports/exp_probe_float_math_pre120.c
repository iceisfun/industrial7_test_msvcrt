/* TC
 * id:    exp_probe_float_math_pre120
 * area:  exports
 * funcs:
 * procs: sinf cosf sqrtf floorf ceilf fmodf powf
 * crt:   msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs sin, sinf; math.h
 * note:  Probing: x64 msvcr90/100/110 export classic float math (sinf etc.); on x86 these were only inline wrappers.
 *        Not claimed for msvcr80 / msvcrt. fabsf is deliberately NOT probed (it is an inline wrapper in the headers, not an export).
 *        One case per name so a wrong guess for one function does not hide the others.
 */
#include "tc.h"

TC_CASE(sinf_exported) { CHECK(tc_crt_proc("sinf") != NULL); }
TC_CASE(cosf_exported) { CHECK(tc_crt_proc("cosf") != NULL); }
TC_CASE(sqrtf_exported) { CHECK(tc_crt_proc("sqrtf") != NULL); }
TC_CASE(floorf_exported) { CHECK(tc_crt_proc("floorf") != NULL); }
TC_CASE(ceilf_exported) { CHECK(tc_crt_proc("ceilf") != NULL); }
TC_CASE(fmodf_exported) { CHECK(tc_crt_proc("fmodf") != NULL); }
TC_CASE(powf_exported) { CHECK(tc_crt_proc("powf") != NULL); }
int main(void) { tc_begin(); TC_RUN(sinf_exported); TC_RUN(cosf_exported); TC_RUN(sqrtf_exported); TC_RUN(floorf_exported); TC_RUN(ceilf_exported); TC_RUN(fmodf_exported); TC_RUN(powf_exported); return tc_end(); }
