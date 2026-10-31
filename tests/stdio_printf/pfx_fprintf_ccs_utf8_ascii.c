/* TC
 * id:    pfx_fprintf_ccs_utf8_ascii
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fopen (ccs=UTF-8)
 * note:  PROBE: narrow fprintf to a ccs=UTF-8 stream. Narrow (ANSI) stdio output functions validate that the stream is not
 *        in a Unicode mode (_VALIDATE_STREAM_ANSI_RETURN), so the expectation is rejection: return -1, errno EINVAL, one
 *        invalid-parameter-handler call (handler installed so the process survives). If real Windows instead writes
 *        BOM + ASCII with CR LF the byte-level outcome is printed as TC INFO.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

#define FN "tc_pfx_fprintf_ccs_utf8_ascii_a_" TC_CRT_DLL ".tmp"

TC_CASE(ascii_rejected)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w, ccs=UTF-8"); int n, r; char t[24]; int i;
    volatile int v = 1;
    CHECK(f != NULL); if (!f) return;
    if (!tc_iph_install()) TC_SKIP("no _set_invalid_parameter_handler");
    tc_iph_count = 0; errno = 0;
    r = fprintf(f, "abc %d\n", v);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    tc_write("TC INFO " TC_TEST_ID " bytes:");
    for (i = 0; i < n; i++) { tc_hex(b[i], t); tc_write(" "); tc_write(t); }
    tc_write("\n");
    remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(ascii_rejected); return tc_end(); }
