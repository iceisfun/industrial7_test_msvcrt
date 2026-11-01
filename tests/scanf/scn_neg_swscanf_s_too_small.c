/* TC
 * id:    scn_neg_swscanf_s_too_small
 * area:  scanf
 * kind:  negative
 * funcs: swscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs wscanf_s, swscanf_s: size is in wchar_t for %ls/%lc, in bytes for %hs; too small invokes the invalid parameter handler
 * note:  CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): a too-small _s destination does NOT call the invalid parameter handler (tc_iph_count stays 0);
 *        the sentinel/overflow checks below still hold. MS docs do not document a handler call for this case. Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (>=90) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <wchar.h>

static const wchar_t *volatile vin;
TC_CASE(ls_too_small)
{
    wchar_t a[8]; int i; for (i = 0; i < 8; i++) a[i] = L'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = L"abcdef";
    (void)swscanf_s((const wchar_t *)vin, L"%ls", a, (unsigned)4);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_U(a[4], L'Z'); CHECK_EQ_U(a[7], L'Z');
}
TC_CASE(lc_too_small)
{
    wchar_t a[8]; int i; for (i = 0; i < 8; i++) a[i] = L'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = L"abcdefg";
    (void)swscanf_s((const wchar_t *)vin, L"%5lc", a, (unsigned)3);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_U(a[3], L'Z'); CHECK_EQ_U(a[5], L'Z');
}
TC_CASE(hs_too_small)
{
    char a[8]; int i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = L"abcdef";
    (void)swscanf_s((const wchar_t *)vin, L"%hs", a, (unsigned)4);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(a[4], 'Z'); CHECK_EQ_I(a[7], 'Z');
}
int main(void) { tc_begin(); TC_RUN(ls_too_small); TC_RUN(lc_too_small); TC_RUN(hs_too_small); return tc_end(); }
