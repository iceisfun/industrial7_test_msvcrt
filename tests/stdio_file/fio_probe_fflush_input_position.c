/* TC
 * id:    fio_probe_fflush_input_position
 * area:  stdio_file
 * funcs: fflush fopen fgetc ftell fclose
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs fflush
 * note:  PROBE: fflush on an input stream discards the buffer; logical position (ftell) and the next fgetc are expected unchanged.\n */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static const char *fn = "tc_fio_probe_fflush_input_position_a.tmp";

TC_CASE(position)
{
    FILE *fp = fopen(fn, "wb");
    volatile int c1, c2;
    CHECK(fp != NULL); if (!fp) return;
    fwrite("0123456789", 1, 10, fp); fclose(fp);
    fp = fopen(fn, "rb");
    CHECK(fp != NULL); if (!fp) return;
    c1 = fgetc(fp);
    CHECK_EQ_I(c1, '0');
    fflush(fp);
    CHECK_EQ_I(ftell(fp), 1);
    c2 = fgetc(fp);
    CHECK_EQ_I(c2, '1');
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(position); return tc_end(); }
