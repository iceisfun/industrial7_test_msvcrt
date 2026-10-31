/* TC
 * id:    mf_neg_fabsf_export_probe
 * area:  math_float
 * funcs:
 * procs: fabsf frexpf ldexpf sinf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fabs, frexp, ldexp, sin (Requirements tables)
 * note:  PROBE: which f-variants are exported by each CRT. Logs presence via "TC PROBE" lines (no assertion on presence);
 *        only asserts GetProcAddress does not crash and that sinf (if present) returns 0 for 0.
 */
#include "tc.h"

static void probe(const char *name)
{
    FARPROC p = tc_crt_proc(name);
    tc_write("TC PROBE " TC_TEST_ID " " TC_CRT_DLL " "); tc_write(name); tc_write(p ? " present\n" : " absent\n");
}
TC_CASE(log_exports) { probe("fabsf"); probe("frexpf"); probe("ldexpf"); probe("sinf"); probe("_hypotf"); probe("hypotf"); CHECK(1); }
TC_CASE(sinf_zero)
{
    float (__cdecl *p)(float) = (float (__cdecl *)(float))tc_crt_proc("sinf");
    volatile float z = 0.0f;
    if (!p) TC_SKIP("sinf not exported");
    CHECK_BITS_F(p(z), 0.0f);
}

int main(void) { tc_begin(); TC_RUN(log_exports); TC_RUN(sinf_zero); return tc_end(); }
