/* TC
 * id:    scn_neg_sscanf_s_l_too_small
 * area:  scanf
 * kind:  negative
 * funcs: fopen fclose
 * procs: _sscanf_s_l _swscanf_s_l _snscanf_s_l _fscanf_s_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs scanf_s, _scanf_s_l, sscanf_s, _sscanf_s_l, swscanf_s, _snscanf_s_l: size too small -> invalid parameter handler; NULL pointers -> invalid parameter handler, EINVAL
 * note:  Return value and ERANGE for the too-small cases are NOT asserted (see scn_neg_secure_too_small_ret_errno_probe).
 *        Locale argument comes right after the format (before the variable list). A NULL _locale_t means the current locale.
 *        NULL-argument calls run in a child process so that a crash is a clean per-case failure.
 *        On ucrtbase these are header inlines over __stdio_common_*; elsewhere resolved at run time (msvcr80's import lib lacks them).
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): a too-small _s destination does NOT call the invalid parameter handler (tc_iph_count stays 0);
 *        the sentinel/overflow checks below still hold. MS docs do not document a handler call for this case. Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

typedef int (__cdecl *ss_t)(const char *, const char *, _locale_t, ...);
typedef int (__cdecl *sw_t)(const wchar_t *, const wchar_t *, _locale_t, ...);
typedef int (__cdecl *sn_t)(const char *, size_t, const char *, _locale_t, ...);
typedef int (__cdecl *fs_t)(FILE *, const char *, _locale_t, ...);
#if TC_CRT_VER >= 140
#define SSL _sscanf_s_l
#define SWL _swscanf_s_l
#define SNL _snscanf_s_l
#define FSL _fscanf_s_l
#define HAVE_P(x) 1
#else
static ss_t p_ss; static sw_t p_sw; static sn_t p_sn; static fs_t p_fs;
#define SSL p_ss
#define SWL p_sw
#define SNL p_sn
#define FSL p_fs
#define HAVE_P(x) ((x) != NULL)
#endif
#define ALLP (HAVE_P(SSL) && HAVE_P(SWL) && HAVE_P(SNL) && HAVE_P(FSL))
#define FN "tc_scn_neg_sscanf_s_l_too_small_" TC_CRT_DLL ".tmp"

static const char *volatile vs; static const wchar_t *volatile vw; static const char *volatile nullstr; static const wchar_t *volatile nullwstr;

static int child_main(const char *c)
{
    int n, x = 5;
    if (!ALLP) return 90;
    if (!tc_iph_install()) return 91;
    errno = 0;
    if (tc_strequ(c, "in_null")) {
        vs = "%d"; n = SSL(nullstr, vs, NULL, &x);
        return (n == EOF && errno == EINVAL && tc_iph_count >= 1 && x == 5) ? 0 : 10;
    }
    if (tc_strequ(c, "fmt_null")) {
        vw = L"12"; n = SWL(vw, nullwstr, NULL, &x);
        return (n == EOF && errno == EINVAL && tc_iph_count >= 1 && x == 5) ? 0 : 11;
    }
    return 99;
}
static void run_child(const char *name)
{
    DWORD code = 1234; int ok = tc_spawn_self(name, &code, 20000);
    CHECK(ok); CHECK_EQ_U(code, 0);
}
TC_CASE(sscanf_s_l_small_buf)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z';
    if (!ALLP) TC_SKIP("not exported");
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vs = "abcdef";
    n = SSL(vs, "%s", NULL, a, (unsigned)3);
    CHECK_EQ_I(tc_iph_count, 0); (void)n;
    CHECK_EQ_I(a[3], 'Z'); CHECK_EQ_I(a[7], 'Z');
}
TC_CASE(swscanf_s_l_small_buf)
{
    wchar_t a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = L'Z';
    if (!ALLP) TC_SKIP("not exported");
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vw = L"abcdef";
    n = SWL(vw, L"%s", NULL, a, (unsigned)3);
    CHECK_EQ_I(tc_iph_count, 0); (void)n;
    CHECK_EQ_U(a[3], L'Z'); CHECK_EQ_U(a[7], L'Z');
}
TC_CASE(snscanf_s_l_small_buf)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z';
    if (!ALLP) TC_SKIP("not exported");
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vs = "abcdef";
    n = SNL(vs, 6, "%s", NULL, a, (unsigned)3);
    CHECK_EQ_I(tc_iph_count, 0); (void)n;
    CHECK_EQ_I(a[3], 'Z'); CHECK_EQ_I(a[7], 'Z');
}
TC_CASE(fscanf_s_l_small_buf)
{
    char a[8]; int n, i; FILE *f; DWORD w; HANDLE h;
    for (i = 0; i < 8; i++) a[i] = 'Z';
    if (!ALLP) TC_SKIP("not exported");
    h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE); if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, "abcdef\n", 7, &w, NULL); CloseHandle(h);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = FSL(f, "%s", NULL, a, (unsigned)3);
    CHECK_EQ_I(tc_iph_count, 0); (void)n;
    CHECK_EQ_I(a[3], 'Z'); CHECK_EQ_I(a[7], 'Z');
    fclose(f); DeleteFileA(FN);
}
TC_CASE(sscanf_s_l_null_input) { run_child("in_null"); }
TC_CASE(swscanf_s_l_null_format) { run_child("fmt_null"); }
TC_CASE(null_locale_valid_control)
{
    char a[8]; int n, x = 0;
    if (!ALLP) TC_SKIP("not exported");
    CHECK(tc_iph_install()); tc_iph_count = 0; vs = "abc 12";
    n = SSL(vs, "%s %d", NULL, a, (unsigned)8, &x);
    CHECK_EQ_I(n, 2); CHECK_STR(a, "abc"); CHECK_EQ_I(x, 12); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    const char *c; tc_begin();
#if TC_CRT_VER < 140
    p_ss = (ss_t)tc_crt_proc("_sscanf_s_l"); p_sw = (sw_t)tc_crt_proc("_swscanf_s_l");
    p_sn = (sn_t)tc_crt_proc("_snscanf_s_l"); p_fs = (fs_t)tc_crt_proc("_fscanf_s_l");
#endif
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(sscanf_s_l_small_buf); TC_RUN(swscanf_s_l_small_buf); TC_RUN(snscanf_s_l_small_buf);
    TC_RUN(fscanf_s_l_small_buf); TC_RUN(sscanf_s_l_null_input); TC_RUN(swscanf_s_l_null_format);
    TC_RUN(null_locale_valid_control);
    return tc_end();
}
