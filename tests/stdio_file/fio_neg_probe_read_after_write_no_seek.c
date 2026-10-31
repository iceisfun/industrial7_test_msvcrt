/* TC
 * id:    fio_neg_probe_read_after_write_no_seek
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fgetc fputc fclose ferror clearerr setvbuf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.5.3p7 (undefined); MS _filbuf legacy
 * note:  PROBE: on an UNBUFFERED (_IONBF) "w+b" stream, fputc then fgetc without intervening seek/fflush returns EOF and sets
 *        ferror (_filbuf rejects a stream with _IOWRT). Unbuffered on purpose: on a buffered stream the getc fast path
 *        (--_cnt >= 0) would just return stale write-buffer bytes without reaching _filbuf.
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <stdio.h>
static const char * volatile fname = "tc_fio_neg_probe_read_after_write_no_seek_a" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(read_after_write)
{
    FILE *fp;
    fp = fopen(fname, "w+b"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(setvbuf(fp, NULL, _IONBF, 0), 0);
    CHECK_EQ_I(fputc('A', fp), 'A');
    CHECK_EQ_I(fgetc(fp), EOF);
    CHECK(ferror(fp) != 0);
    clearerr(fp);
    fclose(fp);
}
int main(void)
{
    tc_begin();
    TC_RUN(read_after_write);
    DeleteFileA(fname);
    return tc_end();
}
