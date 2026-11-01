/* TC
 * id:    pfx_probe_fprintf_ccs_utf8_nonascii_narrow
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   >=140
 * tier:  p2
 * conf:  observed
 * ref:   MS docs fopen (ccs=UTF-8 writes a BOM; stream is wide-oriented)
 * note:  PROBE: narrow fprintf of bytes C3 A9 to a "w, ccs=UTF-8" stream in the C locale. OBSERVED on Win10 22H2 19045: the narrow write of non-ASCII bytes fails (return -1) and only the
 *        EF BB BF BOM reaches the file. errno is EINVAL. Observed bytes are printed as TC INFO.
 *        crt narrowed to ucrtbase: observed only there [msvcr100..120 unverified].
 *        A (test bug): observed r == -1 and file == BOM only.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile path = "tc_pfx_probe_fprintf_ccs_narrow_a.tmp";
static const char *volatile str = "\xC3\xA9";

static DWORD slurp(unsigned char *b, DWORD max)
{
    HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD n = 0;
    if (h == INVALID_HANDLE_VALUE) return 0;
    ReadFile(h, b, max, &n, NULL); CloseHandle(h); return n;
}
static void info(const unsigned char *b, DWORD n)
{
    char t[24]; DWORD i; tc_write("TC INFO " TC_TEST_ID " bytes:");
    for (i = 0; i < n; i++) { tc_hex(b[i], t); tc_write(" "); tc_write(t); }
    tc_write("\n");
}
TC_CASE(narrow_bytes)
{
    unsigned char b[32] = {0}; DWORD n; volatile int r;
    FILE *f = fopen(path, "w, ccs=UTF-8");
    CHECK(f != NULL);
    if (!f) return;
    errno = 0;
    r = fprintf(f, "%s", str);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    fclose(f);
    n = slurp(b, sizeof b); info(b, n);
    CHECK_EQ_U(n, 3);
    if (n == 3) {
        static const unsigned char want[] = { 0xEF, 0xBB, 0xBF };
        CHECK_MEM(b, want, 3);
    }
    DeleteFileA(path);
}
int main(void) { tc_begin(); TC_RUN(narrow_bytes); return tc_end(); }
