/* TC
 * id:    scn_fwscanf_ccs_utf8
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
#define FN "tc_scn_ccs8_" TC_CRT_DLL ".tmp"
TC_CASE(two_byte) { static const unsigned char d[] = { 0xEF,0xBB,0xBF, 0xC3,0xA9, ' ', '5' }; FILE *f; int a = 0, n; wchar_t c = 0; wfile(FN, d, sizeof d); f = fopen(FN, "r,ccs=UTF-8"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%lc %d", &c, &a); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_EQ_U(c, 0x00E9); CHECK_EQ_I(a, 5); }
TC_CASE(three_byte) { static const unsigned char d[] = { 0xEF,0xBB,0xBF, 0xE4,0xB8,0xAD, ' ', '7' }; FILE *f; int a = 0, n; wchar_t c = 0; wfile(FN, d, sizeof d); f = fopen(FN, "r,ccs=UTF-8"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%lc %d", &c, &a); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_EQ_U(c, 0x4E2D); CHECK_EQ_I(a, 7); }
TC_CASE(no_bom_explicit) { static const unsigned char d[] = { 0xC3,0xA9, ' ', '8' }; FILE *f; int a = 0, n; wchar_t c = 0; wfile(FN, d, sizeof d); f = fopen(FN, "r,ccs=UTF-8"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%lc %d", &c, &a); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_EQ_U(c, 0x00E9); CHECK_EQ_I(a, 8); }
int main(void) { tc_begin(); TC_RUN(two_byte); TC_RUN(three_byte); TC_RUN(no_bom_explicit); return tc_end(); }
