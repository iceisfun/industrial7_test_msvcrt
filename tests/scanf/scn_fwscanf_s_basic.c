/* TC
 * id:    scn_fwscanf_s_basic
 * area:  scanf
 * funcs: fwscanf_s fopen fclose
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fwscanf_s, _fwscanf_s_l; sizes are in wchar_t for %s/%c in wide functions
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FN "tc_scn_fwscanf_s_" TC_CRT_DLL ".tmp"
TC_CASE(ls_size) { FILE *f; wchar_t a[8], b[4]; int n, i; for (i = 0; i < 8; i++) a[i] = L'Z'; wfile(FN, "abc def\n", 8); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf_s(f, L"%ls %ls", a, (unsigned)8, b, (unsigned)4); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_WSTR(a, L"abc"); CHECK_WSTR(b, L"def"); CHECK_EQ_U(a[4], L'Z'); }
TC_CASE(lc_size) { FILE *f; wchar_t c = L'Z'; int n, x = 0; wfile(FN, "7Q", 2); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf_s(f, L"%d%lc", &x, &c, (unsigned)1); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_EQ_I(x, 7); CHECK_EQ_U(c, L'Q'); }
TC_CASE(eof) { FILE *f; wchar_t a[4]; int n; wfile(FN, "", 0); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf_s(f, L"%ls", a, (unsigned)4); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, EOF); }
int main(void) { tc_begin(); TC_RUN(ls_size); TC_RUN(lc_size); TC_RUN(eof); return tc_end(); }
