/* TC
 * id:    scn_neg_fwscanf_ccs_bad_utf8_probe
 * area:  scanf
 * kind:  negative
 * funcs: fwscanf fopen fclose
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs fopen (ccs=UTF-8), fgetwc (EILSEQ for invalid multibyte data)
 * note:  PROBE: file "A", 0xFF, "B" opened r,ccs=UTF-8 (0xFF is never valid in UTF-8). Probed in separate cases:
 *        the first %lc reads 'A'; reading the second character does not return 'B' (the invalid byte is not skipped
 *        silently into a valid char) and the call yields EOF (input failure); and errno becomes EILSEQ.
 *        Restricted to ucrtbase: pre-UCRT CRTs convert UTF-8 text with MultiByteToWideChar (no EILSEQ; likely U+FFFD) so the outcome differs.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): an invalid UTF-8 byte (0xFF) under ccs=UTF-8 is returned as U+FFFD (r==1), the next read continues with 'B', and errno stays 0
 *        (no EILSEQ, no EOF). The old expectation (EOF + EILSEQ, dest untouched) was wrong.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FN "tc_scn_neg_bad8.tmp"
static const unsigned char D[] = { 'A', 0xFF, 'B' };
static FILE *mk(void) { FILE *f; wfile(FN, D, sizeof D); f = fopen(FN, "r,ccs=UTF-8"); if (!f) DeleteFileA(FN); return f; }

TC_CASE(first_char) { FILE *f = mk(); wchar_t c = 0; int n; CHECK(f != NULL); if (!f) return; n = fwscanf(f, L"%lc", &c); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 1); CHECK_EQ_U(c, 'A'); }
TC_CASE(second_replacement_char)
{
    FILE *f = mk(); wchar_t c = 0, d = 0x1234, e = 0; int n; CHECK(f != NULL); if (!f) return;
    n = fwscanf(f, L"%lc", &c); CHECK_EQ_I(n, 1);
    n = fwscanf(f, L"%lc", &d); CHECK_EQ_I(n, 1); CHECK_EQ_U(d, 0xFFFD);
    n = fwscanf(f, L"%lc", &e); CHECK_EQ_I(n, 1); CHECK_EQ_U(e, 0x42);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(errno_unchanged)
{
    FILE *f = mk(); wchar_t c = 0; int n; CHECK(f != NULL); if (!f) return;
    n = fwscanf(f, L"%lc", &c); CHECK_EQ_I(n, 1);
    errno = 0; (void)fwscanf(f, L"%lc", &c); fclose(f); DeleteFileA(FN);
    CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); TC_RUN(first_char); TC_RUN(second_replacement_char); TC_RUN(errno_unchanged); return tc_end(); }
