/* TC
 * id:    fio_neg_probe_setvbuf_after_io
 * area:  stdio_file
 * funcs: setvbuf fopen fwrite fputc fgetc fclose
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.5.6 (UB after I/O); MS docs setvbuf
 * note:  PROBE: whether setvbuf after an I/O operation on the stream fails. C11 says UB; guess is that the CRT does not check and returns 0. Test is the probe; no documented assertions.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>


static const char *fn = "tc_fio_neg_probe_setvbuf_after_io_a.tmp";
static char b[1024];

TC_CASE(after_write)
{
    FILE *fp = fopen(fn, "wb"); volatile int r;
    CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(fputc('a', fp), 'a');
    r = setvbuf(fp, b, _IOFBF, sizeof b);
    CHECK_EQ_I(r, 0);
    fclose(fp); DeleteFileA(fn);
}
TC_CASE(after_read)
{
    FILE *fp = fopen(fn, "wb"); volatile int r;
    CHECK(fp != NULL); if (!fp) return;
    fwrite("abc", 1, 3, fp); fclose(fp);
    fp = fopen(fn, "rb");
    CHECK(fp != NULL); if (!fp) { DeleteFileA(fn); return; }
    CHECK_EQ_I(fgetc(fp), 'a');
    r = setvbuf(fp, b, _IOFBF, sizeof b);
    CHECK_EQ_I(r, 0);
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(after_write); TC_RUN(after_read); return tc_end(); }
