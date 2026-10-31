/* TC
 * id:    scn_fscanf_l_basic
 * area:  scanf
 * funcs: _fscanf_l _fwscanf_l fopen fclose _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fscanf, _fscanf_l, fwscanf, _fwscanf_l, _create_locale
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
static void wr(const char *name, const void *d, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, d, n, &w, NULL); CloseHandle(h);
}
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_scn_fscanf_l_basic_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(narrow_null_locale)
{
    FILE *f; int n = 0, r; char s[16] = "";
    wr(F, "12 abc", 6);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = _fscanf_l(f, "%d %15s", NULL, &n, s);
    fclose(f); remove(F);
    CHECK_EQ_I(r, 2); CHECK_EQ_I(n, 12); CHECK_STR(s, "abc");
}
TC_CASE(narrow_c_locale)
{
    FILE *f; int n = 0, r; char s[16] = ""; _locale_t loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL); if (!loc) return;
    wr(F, "12 abc", 6);
    f = fopen(F, "r"); CHECK(f != NULL);
    if (f) { r = _fscanf_l(f, "%d %15s", loc, &n, s); fclose(f); CHECK_EQ_I(r, 2); CHECK_EQ_I(n, 12); CHECK_STR(s, "abc"); }
    remove(F); _free_locale(loc);
}
#include <stdarg.h>
#if TC_AT_LEAST(140)
/* ucrt exports no _fwscanf_l; the header-inline form forwards to __stdio_common_vfwscanf (options 0 = default) */
static int wscanf_l_helper(FILE *f, const wchar_t *fmt, _locale_t loc, ...)
{
    int r; va_list ap; va_start(ap, loc);
    r = (int)__stdio_common_vfwscanf(0, f, fmt, loc, ap);
    va_end(ap); return r;
}
#define fwscanf_l_call wscanf_l_helper
#else
#define fwscanf_l_call _fwscanf_l
#endif
TC_CASE(wide)
{
    FILE *f; int n = 0, r; wchar_t s[16] = L""; _locale_t loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL); if (!loc) return;
    wr(F, "12 abc", 6);
    f = fopen(F, "r"); CHECK(f != NULL);
    if (f) { r = fwscanf_l_call(f, L"%d %15ls", loc, &n, s); fclose(f); CHECK_EQ_I(r, 2); CHECK_EQ_I(n, 12); CHECK_WSTR(s, L"abc"); }
    remove(F); _free_locale(loc);
}
int main(void) { tc_begin(); TC_RUN(narrow_null_locale); TC_RUN(narrow_c_locale); TC_RUN(wide); return tc_end(); }
