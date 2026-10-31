/* TC
 * id:    scn_fwscanf_ccs_utf16le
 * area:  scanf
 * funcs: fwscanf fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fopen, _wfopen (ccs=UNICODE|UTF-8|UTF-16LE: a BOM in an existing file overrides ccs; ccs applies when no BOM is present)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FN "tc_scn_ccs16_" TC_CRT_DLL ".tmp"
TC_CASE(bom) { static const unsigned char d[] = { 0xFF,0xFE, '1',0, '2',0, ' ',0, 0x2D,0x4E }; FILE *f; int a = 0, n; wchar_t c = 0; wfile(FN, d, sizeof d); f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%d %lc", &a, &c); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 12); CHECK_EQ_U(c, 0x4E2D); }
TC_CASE(no_bom_explicit) { static const unsigned char d[] = { '3',0, '4',0, ' ',0, 0x2D,0x4E }; FILE *f; int a = 0, n; wchar_t c = 0; wfile(FN, d, sizeof d); f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%d %lc", &a, &c); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 34); CHECK_EQ_U(c, 0x4E2D); }
TC_CASE(ls_string) { static const unsigned char d[] = { 0xFF,0xFE, 'h',0, 'i',0, ' ',0, '9',0 }; FILE *f; int a = 0, n; wchar_t s[8]; wfile(FN, d, sizeof d); f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%ls %d", s, &a); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_WSTR(s, L"hi"); CHECK_EQ_I(a, 9); }
int main(void) { tc_begin(); TC_RUN(bom); TC_RUN(no_bom_explicit); TC_RUN(ls_string); return tc_end(); }
