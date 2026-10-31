/* TC
 * id:    fio_ftell_after_ungetc_binary
 * area:  stdio_file
 * funcs: ftell ungetc fgetc fopen fread fclose
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10 ungetc; 7.21.9.4 ftell
 * note:  
 */
#include "tc.h"
#include <stdio.h>

static char nm[] = "tc_fio_ftell_after_ungetc_binary_000.tmp";  /* per-CRT suffix: targets may run concurrently */
static void setnm(void) { int v = TC_CRT_VER; char *p = nm + sizeof nm - 9; p[0]=(char)('0'+v/100); p[1]=(char)('0'+v/10%10); p[2]=(char)('0'+v%10); }

TC_CASE(unget_adjusts)
{
    char b[8]; int c;
    FILE *f = fopen(nm, "wb"); CHECK(f != NULL); if (!f) return;
    fwrite("abcde", 1, 5, f); fclose(f);
    f = fopen(nm, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(b, 1, 3, f), 3);
    CHECK_EQ_I(ftell(f), 3);
    CHECK_EQ_I(ungetc('c', f), 'c');
    CHECK_EQ_I(ftell(f), 2);
    c = fgetc(f); CHECK_EQ_I(c, 'c');
    CHECK_EQ_I(ftell(f), 3);
    c = fgetc(f); CHECK_EQ_I(c, 'd');
    CHECK_EQ_I(ftell(f), 4);
    fclose(f);
}
int main(void) { tc_begin(); setnm(); TC_RUN(unget_adjusts); remove(nm); return tc_end(); }
