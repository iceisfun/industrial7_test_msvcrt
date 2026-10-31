/* TC
 * id:    scn_fwscanf_basic
 * area:  scanf
 * funcs: fwscanf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fscanf, fwscanf; C11 7.29.2.2 (ANSI/ASCII file opened in mode r; %ls %lc; EOF return)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FN "tc_scn_fwscanf_basic_" TC_CRT_DLL ".tmp"
TC_CASE(ints) { FILE *f; int a = 0, b = 0, n; wfile(FN, "12 -7\n", 6); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%d %d", &a, &b); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, -7); }
TC_CASE(ls_lc) { FILE *f; wchar_t s[8], c = L'Q'; int n; wfile(FN, "abc Z\n", 6); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%ls %lc", s, &c); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_WSTR(s, L"abc"); CHECK_EQ_U(c, L'Z'); }
TC_CASE(crlf_text) { FILE *f; int a = 0, b = 0, n; wfile(FN, "1\r\n2\r\n", 6); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%d %d", &a, &b); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 2); }
TC_CASE(eof) { FILE *f; int a = 0, n, m; wfile(FN, "5\n", 2); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%d", &a); m = fwscanf(f, L"%d", &a); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 1); CHECK_EQ_I(m, EOF); }
TC_CASE(empty_file_eof) { FILE *f; int a = 0, n; wfile(FN, "", 0); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%d", &a); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, EOF); }
int main(void) { tc_begin(); TC_RUN(ints); TC_RUN(ls_lc); TC_RUN(crlf_text); TC_RUN(eof); TC_RUN(empty_file_eof); return tc_end(); }
