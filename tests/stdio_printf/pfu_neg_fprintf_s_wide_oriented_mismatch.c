/* TC
 * id:    pfu_neg_fprintf_s_wide_oriented_mismatch
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s fwprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.2 (orientation) vs MS CRT (no orientation lock)
 * note:  Probing exactly: MS CRTs do not enforce stream orientation, so narrow and wide output may be mixed on one
 *        text-mode stream; both calls return their character count (3) and the invalid-parameter handler does not run.
 *        This is a "should NOT fail" check. Temp file tc_pfu_neg_fprintf_s_mix_*.tmp, removed.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define FN "tc_pfu_neg_fprintf_s_mix_" TC_CRT_DLL ".tmp"

TC_CASE(mixed_orientation_succeeds)
{
    FILE *f = fopen(FN, "w"); int r1, r2;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    r1 = fprintf_s(f, "abc");
    r2 = fwprintf_s(f, L"def");
    CHECK_EQ_I(r1, 3); CHECK_EQ_I(r2, 3);
    CHECK_EQ_I(tc_iph_count, 0);
    fclose(f);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(mixed_orientation_succeeds);
    DeleteFileA(FN);
    return tc_end();
}
