/* TC
 * id:    scn_sscanf_s_l_variants
 * area:  scanf
 * funcs: _sscanf_s_l _swscanf_s_l _snscanf_s_l _snwscanf_s_l _fscanf_s_l _fwscanf_s_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs scanf_s, _scanf_s_l, wscanf_s, _wscanf_s_l (locale is the argument before the variable list; _sn variants take count first)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FNA "tc_scn_sl_a_" TC_CRT_DLL ".tmp"
#define FNW "tc_scn_sl_w_" TC_CRT_DLL ".tmp"
static volatile const char *vs; static volatile const wchar_t *vw;
static _locale_t loc;
TC_CASE(sscanf_s_l) { char a[8]; int x = 0, n; vs = "abc 12"; n = _sscanf_s_l((const char *)vs, "%s %d", loc, a, (unsigned)8, &x); CHECK_EQ_I(n, 2); CHECK_STR(a, "abc"); CHECK_EQ_I(x, 12); }
TC_CASE(swscanf_s_l) { wchar_t a[8]; int x = 0, n; vw = L"abc 12"; n = _swscanf_s_l((const wchar_t *)vw, L"%s %d", loc, a, (unsigned)8, &x); CHECK_EQ_I(n, 2); CHECK_WSTR(a, L"abc"); CHECK_EQ_I(x, 12); }
TC_CASE(snscanf_s_l) { char a[8]; int x = 0, n; vs = "abc 12 99"; n = _snscanf_s_l((const char *)vs, 6, "%s %d", loc, a, (unsigned)8, &x); CHECK_EQ_I(n, 2); CHECK_STR(a, "abc"); CHECK_EQ_I(x, 12); }
TC_CASE(snwscanf_s_l) { wchar_t a[8]; int x = 0, n; vw = L"abc 12 99"; n = _snwscanf_s_l((const wchar_t *)vw, 6, L"%s %d", loc, a, (unsigned)8, &x); CHECK_EQ_I(n, 2); CHECK_WSTR(a, L"abc"); CHECK_EQ_I(x, 12); }
TC_CASE(fscanf_s_l) { FILE *f; char a[8]; int x = 0, n; wfile(FNA, "abc 12\n", 7); f = fopen(FNA, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FNA); return; } n = _fscanf_s_l(f, "%s %d", loc, a, (unsigned)8, &x); fclose(f); DeleteFileA(FNA); CHECK_EQ_I(n, 2); CHECK_STR(a, "abc"); CHECK_EQ_I(x, 12); }
TC_CASE(fwscanf_s_l) { FILE *f; wchar_t a[8]; int x = 0, n; wfile(FNW, "abc 12\n", 7); f = fopen(FNW, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FNW); return; } n = _fwscanf_s_l(f, L"%s %d", loc, a, (unsigned)8, &x); fclose(f); DeleteFileA(FNW); CHECK_EQ_I(n, 2); CHECK_WSTR(a, L"abc"); CHECK_EQ_I(x, 12); }
static void run_all(void) { TC_RUN(sscanf_s_l); TC_RUN(swscanf_s_l); TC_RUN(snscanf_s_l); TC_RUN(snwscanf_s_l); TC_RUN(fscanf_s_l); TC_RUN(fwscanf_s_l); }
int main(void)
{
    tc_begin();
    loc = NULL; run_all();                 /* NULL locale = current (C) locale */
    loc = _create_locale(LC_ALL, "C"); CHECK(loc != NULL);
    if (loc) { run_all(); _free_locale(loc); }
    return tc_end();
}
