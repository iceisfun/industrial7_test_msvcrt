/* TC
 * id:    scn_secure_msvcr80_procs
 * area:  scanf
 * funcs: fopen fclose
 * procs: sscanf_s swscanf_s _snscanf_s fscanf_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs scanf_s, _scanf_s_l, wscanf_s, _snscanf_s, _snwscanf_s, fscanf_s (VS2005+)
 * note:  mingw's libmsvcr80.a lacks the _s scanf variants although the real DLLs export them, so they are resolved with
 *        GetProcAddress. Every case skips if the symbol is missing. ucrtbase is excluded: there the _s functions are inline
 *        wrappers over __stdio_common_vsscanf (see scn_common_vsscanf_secure_flag).
 */
#include "tc.h"
#include <stdio.h>
#define STR_(x) #x
#define XSTR_(x) STR_(x)

typedef int (__cdecl *sscanf_s_t)(const char *, const char *, ...);
typedef int (__cdecl *swscanf_s_t)(const wchar_t *, const wchar_t *, ...);
typedef int (__cdecl *snscanf_s_t)(const char *, size_t, const char *, ...);
typedef int (__cdecl *fscanf_s_t)(FILE *, const char *, ...);
static sscanf_s_t p_sscanf_s; static swscanf_s_t p_swscanf_s; static snscanf_s_t p_snscanf_s; static fscanf_s_t p_fscanf_s;
static const char *volatile vin;

TC_CASE(exported)
{
    CHECK(p_sscanf_s != NULL); CHECK(p_swscanf_s != NULL); CHECK(p_snscanf_s != NULL); CHECK(p_fscanf_s != NULL);
}
TC_CASE(s_fits)
{
    char b[8]; int i, n, v = 0;
    if (!p_sscanf_s) TC_SKIP("sscanf_s not exported");
    for (i = 0; i < 8; i++) b[i] = 'Z';
    vin = "42 abc";
    n = p_sscanf_s((const char *)vin, "%d %s", &v, b, (unsigned)8);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(v, 42); CHECK_STR(b, "abc");
    for (i = 0; i < 8; i++) b[i] = 'Z';
    vin = "abc";   /* 3 chars + NUL needs exactly 4 */
    n = p_sscanf_s((const char *)vin, "%s", b, (unsigned)4);
    CHECK_EQ_I(n, 1); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(c_size)
{
    char b[4]; int n;
    if (!p_sscanf_s) TC_SKIP("sscanf_s not exported");
    b[0] = b[1] = b[2] = b[3] = 'Z';
    vin = "xyz";
    n = p_sscanf_s((const char *)vin, "%2c", b, (unsigned)2);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 'x'); CHECK_EQ_I(b[1], 'y'); CHECK_EQ_I(b[2], 'Z');
}
TC_CASE(wide_s)
{
    wchar_t b[8]; int n, v = 0;
    if (!p_swscanf_s) TC_SKIP("swscanf_s not exported");
    n = p_swscanf_s(L"7 hello", L"%d %s", &v, b, (unsigned)8);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(v, 7); CHECK_WSTR(b, L"hello");
}
TC_CASE(snscanf_s_count)
{
    int v = 0, n;
    if (!p_snscanf_s) TC_SKIP("_snscanf_s not exported");
    vin = "12345";
    n = p_snscanf_s((const char *)vin, 3, "%d", &v);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(v, 123);
    v = 0; n = p_snscanf_s((const char *)vin, 5, "%d", &v);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(v, 12345);
}
TC_CASE(fscanf_s_file)
{
    const char *name = "tc_scn_secure_msvcr80_procs_" XSTR_(TC_CRT_VER) ".tmp"; HANDLE h; DWORD w; FILE *f; char b[8]; int v = 0, n;
    if (!p_fscanf_s) TC_SKIP("fscanf_s not exported");
    h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE); if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, "15 word\n", 8, &w, NULL); CloseHandle(h);
    f = fopen(name, "r"); CHECK(f != NULL);
    if (f) { n = p_fscanf_s(f, "%d %s", &v, b, (unsigned)8); CHECK_EQ_I(n, 2); CHECK_EQ_I(v, 15); CHECK_STR(b, "word"); fclose(f); }
    DeleteFileA(name);
}
int main(void)
{
    tc_begin();
    p_sscanf_s = (sscanf_s_t)tc_crt_proc("sscanf_s"); p_swscanf_s = (swscanf_s_t)tc_crt_proc("swscanf_s");
    p_snscanf_s = (snscanf_s_t)tc_crt_proc("_snscanf_s"); p_fscanf_s = (fscanf_s_t)tc_crt_proc("fscanf_s");
    TC_RUN(exported); TC_RUN(s_fits); TC_RUN(c_size); TC_RUN(wide_s); TC_RUN(snscanf_s_count); TC_RUN(fscanf_s_file);
    return tc_end();
}
