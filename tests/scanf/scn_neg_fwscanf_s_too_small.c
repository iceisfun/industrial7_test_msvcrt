/* TC
 * id:    scn_neg_fwscanf_s_too_small
 * area:  scanf
 * kind:  negative
 * funcs: fopen fclose
 * procs: fwscanf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs scanf_s, wscanf_s: buffer size too small invokes the invalid parameter handler (no write past the given size)
 * note:  fwscanf_s is a header inline over __stdio_common_vfwscanf_s on ucrtbase, resolved by proc elsewhere (msvcr80 import lib lacks it).
 *        Wide format: %s and %c are wchar_t, sizes count wchar_t elements. Return value, errno and buffer-emptying are NOT asserted here
 *        (see scn_neg_secure_too_small_ret_errno_probe); nor is how much input is consumed after failure.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): a too-small _s destination does NOT call the invalid parameter handler (tc_iph_count stays 0);
 *        the sentinel/overflow checks below still hold. MS docs do not document a handler call for this case. Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define FN "tc_scn_neg_fwscanf_s_too_small_" TC_CRT_DLL ".tmp"
typedef int (__cdecl *fw_t)(FILE *, const wchar_t *, ...);
static fw_t p_fwscanf_s;
#if TC_CRT_VER < 140
#define fwscanf_s p_fwscanf_s
#endif

static void wfile(const char *text, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, text, len, &w, NULL); CloseHandle(h); }
}
static const wchar_t *volatile vf;
static void fill(wchar_t *a) { int i; for (i = 0; i < 8; i++) a[i] = L'Z'; }

TC_CASE(s_too_small_iph)
{
    wchar_t a[8]; FILE *f; int n; fill(a); wfile("abcdef\n", 7);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vf = L"%s";
    n = fwscanf_s(f, vf, a, (unsigned)3);
    CHECK_EQ_I(tc_iph_count, 0); (void)n; (void)errno;
    fclose(f); DeleteFileA(FN);
}
TC_CASE(s_sentinel_intact)
{
    wchar_t a[8]; FILE *f; fill(a); wfile("abcdef\n", 7);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    CHECK(tc_iph_install()); tc_iph_count = 0; vf = L"%s";
    (void)fwscanf_s(f, vf, a, (unsigned)3);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_U(a[3], L'Z'); CHECK_EQ_U(a[4], L'Z'); CHECK_EQ_U(a[7], L'Z');
    fclose(f); DeleteFileA(FN);
}
TC_CASE(c_width3_size2_iph_sentinel)
{
    wchar_t a[8]; FILE *f; int n; fill(a); wfile("abcdef\n", 7);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vf = L"%3c";
    n = fwscanf_s(f, vf, a, (unsigned)2);
    CHECK_EQ_I(tc_iph_count, 0); (void)n;
    CHECK_EQ_U(a[2], L'Z'); CHECK_EQ_U(a[7], L'Z');
    fclose(f); DeleteFileA(FN);
}
TC_CASE(set_too_small)
{
    wchar_t a[8]; FILE *f; int n; fill(a); wfile("abcdef\n", 7);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vf = L"%[a-z]";
    n = fwscanf_s(f, vf, a, (unsigned)3);
    CHECK_EQ_I(tc_iph_count, 0); (void)n;
    CHECK_EQ_U(a[3], L'Z'); CHECK_EQ_U(a[7], L'Z');
    fclose(f); DeleteFileA(FN);
}
TC_CASE(exact_fit_control_no_iph)
{
    wchar_t a[8]; FILE *f; int n; fill(a); wfile("ab\n", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    CHECK(tc_iph_install()); tc_iph_count = 0; vf = L"%s";
    n = fwscanf_s(f, vf, a, (unsigned)3);
    CHECK_EQ_I(n, 1); CHECK_WSTR(a, L"ab"); CHECK_EQ_I(tc_iph_count, 0);
    fclose(f); DeleteFileA(FN);
}
int main(void)
{
    tc_begin(); p_fwscanf_s = (fw_t)tc_crt_proc("fwscanf_s");
    TC_RUN(s_too_small_iph); TC_RUN(s_sentinel_intact); TC_RUN(c_width3_size2_iph_sentinel);
    TC_RUN(set_too_small); TC_RUN(exact_fit_control_no_iph);
    return tc_end();
}
