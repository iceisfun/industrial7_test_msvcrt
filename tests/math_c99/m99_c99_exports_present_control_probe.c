/* TC
 * id:    m99_c99_exports_present_control_probe
 * area:  math_c99
 * funcs:
 * procs: cbrt lround llrint nearbyint rint fmax fmin fdim exp2 erfc nexttoward tgamma lgamma nan fma lroundf lgammaf tgammaf nanf expm1f log1pf cbrtf truncf roundf fmaf nexttowardf
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Universal C runtime routines by category" (C99 math in CRT 12.0 and UCRT)
 * note:  PROBE: control for m99_neg_c99_exports_missing_probe - the same names resolve on msvcr120 and ucrtbase.
 */
#include "tc.h"
static const char *const n[] = { "cbrt","lround","llrint","nearbyint","rint","fmax","fmin","fdim","exp2","erfc","nexttoward","tgamma","lgamma","nan","fma",
  "lroundf","lgammaf","tgammaf","nanf","expm1f","log1pf","cbrtf","truncf","roundf","fmaf","nexttowardf" };
TC_CASE(all_present)
{
    unsigned i;
    for (i = 0; i < sizeof n / sizeof n[0]; i++)
        if (!tc_crt_proc(n[i])) { tc_failhdr(__FILE__, __LINE__); tc_write("missing export: "); tc_write(n[i]); tc_write("\n"); }
}
int main(void) { tc_begin(); TC_RUN(all_present); return tc_end(); }
