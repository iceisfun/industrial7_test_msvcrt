/* TC
 * id:    scn_fwscanf_ccs_utf8_surrogate
 * area:  scanf
 * funcs: fwscanf fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen, _wfopen (ccs=UNICODE|UTF-8|UTF-16LE: a BOM in an existing file overrides ccs; ccs applies when no BOM is present)
 * note:  Probes: a 4-byte UTF-8 sequence (U+1F600) read through fwscanf %lc %lc yields the UTF-16 surrogate pair D83D, DE00 in two successive wide chars. */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FN "tc_scn_ccs8s_" TC_CRT_DLL ".tmp"
TC_CASE(pair) { static const unsigned char d[] = { 0xEF,0xBB,0xBF, 0xF0,0x9F,0x98,0x80 }; FILE *f; int n; wchar_t c1 = 0, c2 = 0; wfile(FN, d, sizeof d); f = fopen(FN, "r,ccs=UTF-8"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fwscanf(f, L"%lc%lc", &c1, &c2); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_EQ_U(c1, 0xD83D); CHECK_EQ_U(c2, 0xDE00); }
int main(void) { tc_begin(); TC_RUN(pair); return tc_end(); }
