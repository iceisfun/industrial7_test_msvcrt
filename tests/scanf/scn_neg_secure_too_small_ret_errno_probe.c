/* TC
 * id:    scn_neg_secure_too_small_ret_errno_probe
 * area:  scanf
 * kind:  negative
 * funcs: fopen fclose
 * procs: fwscanf_s _snwscanf_s _sscanf_s_l sscanf_s swscanf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  observed
 * ref:   MS docs scanf_s (return value, errno and buffer state after a buffer-too-small failure are not precisely documented)
 * note:  PROBE. With a returning invalid-parameter handler, a size argument too small for the converted data across the wide
 *        file (fwscanf_s), counted (_snwscanf_s), locale (_sscanf_s_l) and %S/%C (sscanf_s, swscanf_s) variants:
 *        expected return 0 (could be EOF), errno ERANGE (could be EINVAL or untouched), and the destination's first element set to 0
 *        (string emptied) for %s. Each guess is its own case.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): a too-small _s destination returns 0 with errno=ENOMEM (12) (not ERANGE) and does NOT call the invalid parameter handler (count 0).
 *        MS docs do not document either. Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

typedef int (__cdecl *fw_t)(FILE *, const wchar_t *, ...);
typedef int (__cdecl *snw_t)(const wchar_t *, size_t, const wchar_t *, ...);
typedef int (__cdecl *ssl_t)(const char *, const char *, _locale_t, ...);
typedef int (__cdecl *ss_t)(const char *, const char *, ...);
typedef int (__cdecl *sw_t)(const wchar_t *, const wchar_t *, ...);
#if TC_CRT_VER >= 140
#define FWS fwscanf_s
#define SNWS _snwscanf_s
#define SSL _sscanf_s_l
#define SS sscanf_s
#define SWS swscanf_s
#define HAVE_P(x) 1
#else
static fw_t p_fw; static snw_t p_snw; static ssl_t p_ssl; static ss_t p_ss; static sw_t p_sw;
#define FWS p_fw
#define SNWS p_snw
#define SSL p_ssl
#define SS p_ss
#define SWS p_sw
#define HAVE_P(x) ((x) != NULL)
#endif
#define FN "tc_scn_neg_secure_ts_probe_" TC_CRT_DLL ".tmp"
static const char *volatile vs; static const wchar_t *volatile vw; static const wchar_t *volatile vf;

static FILE *mkfile(void)
{
    DWORD w; FILE *f; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return NULL;
    WriteFile(h, "abcdef\n", 7, &w, NULL); CloseHandle(h);
    f = fopen(FN, "r"); if (!f) DeleteFileA(FN);
    return f;
}
static void fillw(wchar_t *a) { int i; for (i = 0; i < 8; i++) a[i] = L'Z'; }

TC_CASE(fwscanf_s_ret0)
{
    wchar_t a[8]; FILE *f; int n; fillw(a); if (!HAVE_P(FWS)) TC_SKIP("not exported");
    f = mkfile(); CHECK(f != NULL); if (!f) return;
    CHECK(tc_iph_install()); vf = L"%s"; n = FWS(f, vf, a, (unsigned)3); CHECK_EQ_I(n, 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(fwscanf_s_errno_erange)
{
    wchar_t a[8]; FILE *f; fillw(a); if (!HAVE_P(FWS)) TC_SKIP("not exported");
    f = mkfile(); CHECK(f != NULL); if (!f) return;
    CHECK(tc_iph_install()); vf = L"%s"; errno = 0; (void)FWS(f, vf, a, (unsigned)3); CHECK_EQ_I(errno, ENOMEM);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(fwscanf_s_emptied)
{
    wchar_t a[8]; FILE *f; fillw(a); if (!HAVE_P(FWS)) TC_SKIP("not exported");
    f = mkfile(); CHECK(f != NULL); if (!f) return;
    CHECK(tc_iph_install()); vf = L"%s"; (void)FWS(f, vf, a, (unsigned)3); CHECK_EQ_U(a[0], 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(snwscanf_s_ret0_erange)
{
    wchar_t a[8]; int n; fillw(a); if (!HAVE_P(SNWS)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); errno = 0; vw = L"abcdef"; n = SNWS(vw, 6, L"%s", a, (unsigned)3);
    CHECK_EQ_I(n, 0); CHECK_EQ_I(errno, ENOMEM);
}
TC_CASE(snwscanf_s_emptied)
{
    wchar_t a[8]; fillw(a); if (!HAVE_P(SNWS)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); vw = L"abcdef"; (void)SNWS(vw, 6, L"%s", a, (unsigned)3); CHECK_EQ_U(a[0], 0);
}
TC_CASE(sscanf_s_l_ret0_erange)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z'; if (!HAVE_P(SSL)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); errno = 0; vs = "abcdef"; n = SSL(vs, "%s", NULL, a, (unsigned)3);
    CHECK_EQ_I(n, 0); CHECK_EQ_I(errno, ENOMEM);
}
TC_CASE(sscanf_S_ret0_erange)
{
    wchar_t a[4]; int n, i; for (i = 0; i < 4; i++) a[i] = L'Z'; if (!HAVE_P(SS)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); errno = 0; vs = "abc"; n = SS(vs, "%S", a, (unsigned)2);
    CHECK_EQ_I(n, 0); CHECK_EQ_I(errno, ENOMEM);
}
TC_CASE(swscanf_S_ret0_erange)
{
    char a[4]; int n, i; for (i = 0; i < 4; i++) a[i] = 'Z'; if (!HAVE_P(SWS)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); errno = 0; vw = L"abc"; n = SWS(vw, L"%S", a, (unsigned)2);
    CHECK_EQ_I(n, 0); CHECK_EQ_I(errno, ENOMEM);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    p_fw = (fw_t)tc_crt_proc("fwscanf_s"); p_snw = (snw_t)tc_crt_proc("_snwscanf_s"); p_ssl = (ssl_t)tc_crt_proc("_sscanf_s_l");
    p_ss = (ss_t)tc_crt_proc("sscanf_s"); p_sw = (sw_t)tc_crt_proc("swscanf_s");
#endif
    TC_RUN(fwscanf_s_ret0); TC_RUN(fwscanf_s_errno_erange); TC_RUN(fwscanf_s_emptied);
    TC_RUN(snwscanf_s_ret0_erange); TC_RUN(snwscanf_s_emptied); TC_RUN(sscanf_s_l_ret0_erange);
    TC_RUN(sscanf_S_ret0_erange); TC_RUN(swscanf_S_ret0_erange);
    return tc_end();
}
