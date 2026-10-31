/* TC
 * id:    fio_probe_setvbuf_overflow_partial
 * area:  stdio_file
 * funcs: setvbuf fopen fwrite fclose
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs setvbuf
 * note:  PROBE: after fwrite of 1500 bytes through a 1024-byte user buffer, some data (>0 bytes) has already reached the file
 *        (CRT writes whole buffer-size multiples directly), before any fflush. Implementation detail, not documented.
 */
#include "tc.h"
#include <stdio.h>

static const char *fn = "tc_fio_probe_setvbuf_overflow_partial_a.tmp";
static char buf1[1024];
static char big[1500];

TC_CASE(partial)
{
    FILE *fp = fopen(fn, "wb"); volatile int r; long s = -1; int i;
    HANDLE h;
    CHECK(fp != NULL); if (!fp) return;
    for (i = 0; i < (int)sizeof big; i++) big[i] = 'x';
    r = setvbuf(fp, buf1, _IOFBF, sizeof buf1);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(fwrite(big, 1, sizeof big, fp), sizeof big);
    h = CreateFileA(fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h != INVALID_HANDLE_VALUE) { s = (long)GetFileSize(h, NULL); CloseHandle(h); }
    CHECK(s > 0 && s < (long)sizeof big);
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(partial); return tc_end(); }
