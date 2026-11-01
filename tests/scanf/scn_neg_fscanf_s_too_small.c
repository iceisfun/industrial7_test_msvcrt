/* TC
 * id:    scn_neg_fscanf_s_too_small
 * area:  scanf
 * kind:  negative
 * funcs: fscanf_s fwscanf_s fopen fclose
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fscanf_s, fwscanf_s: buffer size too small invokes the invalid parameter handler
 * note:  How much of the stream is consumed after the failure is deliberately not asserted.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): a too-small _s destination does NOT call the invalid parameter handler (tc_iph_count stays 0);
 *        the sentinel/overflow checks below still hold. MS docs do not document a handler call for this case. Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (>=90) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(narrow)
{
    char a[8]; int i; FILE *f = fopen("tc_scn_neg_fscanf_s_too_small_a.tmp", "wb");
    for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(f != NULL); if (!f) return;
    fwrite("abcdef\n", 1, 7, f); fclose(f);
    f = fopen("tc_scn_neg_fscanf_s_too_small_a.tmp", "r"); CHECK(f != NULL);
    if (f) {
        CHECK(tc_iph_install()); tc_iph_count = 0;
        (void)fscanf_s(f, "%s", a, (unsigned)3);
        CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(a[3], 'Z'); CHECK_EQ_I(a[7], 'Z');
        fclose(f);
    }
    remove("tc_scn_neg_fscanf_s_too_small_a.tmp");
}
TC_CASE(wide)
{
    wchar_t a[8]; int i; FILE *f = fopen("tc_scn_neg_fscanf_s_too_small_w.tmp", "wb");
    for (i = 0; i < 8; i++) a[i] = L'Z';
    CHECK(f != NULL); if (!f) return;
    fwrite("abcdef\n", 1, 7, f); fclose(f);
    f = fopen("tc_scn_neg_fscanf_s_too_small_w.tmp", "r"); CHECK(f != NULL);
    if (f) {
        CHECK(tc_iph_install()); tc_iph_count = 0;
        (void)fwscanf_s(f, L"%s", a, (unsigned)3);
        CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_U(a[3], L'Z'); CHECK_EQ_U(a[7], L'Z');
        fclose(f);
    }
    remove("tc_scn_neg_fscanf_s_too_small_w.tmp");
}
int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
