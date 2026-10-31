/* TC
 * id:    scn_fwscanf_ccs_utf16le_crlf
 * area:  scanf
 * funcs: fwscanf fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen, _wfopen (ccs=UNICODE|UTF-8|UTF-16LE: a BOM in an existing file overrides ccs; ccs applies when no BOM is present)
 * note:  Probes: in text mode with ccs=UTF-16LE, CR LF (0D 00 0A 00) is read as a single L'\\n'. Most likely true (translation is by text mode). */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FN "tc_scn_ccs16crlf_" TC_CRT_DLL ".tmp"
TC_CASE(crlf_to_lf) { static const unsigned char d[] = { 0xFF,0xFE, '5',0, '\r',0, '\n',0, 'X',0 }; FILE *f; int a = 0, n; wchar_t c1 = 1, c2 = 1; wfile(FN, d, sizeof d); f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%d%lc%lc", &a, &c1, &c2); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 3); CHECK_EQ_I(a, 5); CHECK_EQ_U(c1, L'\n'); CHECK_EQ_U(c2, L'X'); }
int main(void) { tc_begin(); TC_RUN(crlf_to_lf); return tc_end(); }
