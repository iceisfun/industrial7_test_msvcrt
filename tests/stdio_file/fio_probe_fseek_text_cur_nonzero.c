/* TC
 * id:    fio_probe_fseek_text_cur_nonzero
 * area:  stdio_file
 * funcs: fseek fopen fgetc
 * procs: 
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.9.2 (SEEK_CUR only offset 0 for text); MS docs fseek
 * note:  PROBED: MS fseek(SEEK_CUR, 2) on a text stream does byte arithmetic, returns 0, next char is the byte at position 3. C11 leaves this undefined/unsupported.
 */
#include "tc.h"
#include <stdio.h>

static char nm[] = "tc_fio_probe_fseek_text_cur_nonzero_000.tmp";  /* per-CRT suffix: targets may run concurrently */
static void setnm(void) { int v = TC_CRT_VER; char *p = nm + sizeof nm - 9; p[0]=(char)('0'+v/100); p[1]=(char)('0'+v/10%10); p[2]=(char)('0'+v%10); }

TC_CASE(seek_cur_2)
{
    FILE *f = fopen(nm, "wb");
    int r, c;
    CHECK(f != NULL); if (!f) return;
    fwrite("a\r\nb\r\nc", 1, 7, f); fclose(f);
    f = fopen(nm, "r"); CHECK(f != NULL); if (!f) return;
    c = fgetc(f); CHECK_EQ_I(c, 'a');
    r = fseek(f, 2, SEEK_CUR);
    CHECK_EQ_I(r, 0);
    if (r == 0) { c = fgetc(f); CHECK_EQ_I(c, 'b'); }
    fclose(f);
}
int main(void) { tc_begin(); setnm(); TC_RUN(seek_cur_2); remove(nm); return tc_end(); }
