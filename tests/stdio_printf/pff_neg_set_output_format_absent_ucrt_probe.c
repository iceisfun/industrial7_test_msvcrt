/* TC
 * id:    pff_neg_set_output_format_absent_ucrt_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _set_output_format _get_output_format
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _set_output_format / _get_output_format (UCRT: two-digit exponent is permanent)
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe2: "exports: _set_output_format=0 _get_output_format=0"): ucrtbase exports neither
 *        _set_output_format nor _get_output_format (both are pre-UCRT msvcr* exports). Original belief that the getter stays exported
 *        was wrong (class D, export-name probe fixed against the real export table). getter_zero is skipped on ucrtbase. Stable.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(setter_absent) { CHECK(tc_crt_proc("_set_output_format") == NULL); }
TC_CASE(getter_absent) { CHECK(tc_crt_proc("_get_output_format") == NULL); }
TC_CASE(getter_zero)
{
    unsigned int (__cdecl *g)(void) = (unsigned int (__cdecl *)(void))tc_crt_proc("_get_output_format");
    if (!g) TC_SKIP("not exported");
    CHECK_EQ_U(g(), 0);   /* never runs on ucrtbase: getter absent too (see getter_absent) */
}

int main(void) { tc_begin(); TC_RUN(setter_absent); TC_RUN(getter_absent); TC_RUN(getter_zero); return tc_end(); }
