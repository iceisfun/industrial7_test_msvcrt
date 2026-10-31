/* TC
 * id:    fio_probe_ftell_append_initial
 * area:  stdio_file
 * funcs: ftell fopen fwrite fclose fflush
 * procs: 
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.5.3p4 (initial position of append stream implementation-defined)
 * note:  PROBED: initial ftell of an 'ab' stream on a 5-byte file is 0 or 5 (either accepted); after writing 3 bytes and fflush the position is 8.
 */
#include "tc.h"
#include <stdio.h>

static char nm[] = "tc_fio_probe_ftell_append_initial_000.tmp";  /* per-CRT suffix: targets may run concurrently */
static void setnm(void) { int v = TC_CRT_VER; char *p = nm + sizeof nm - 9; p[0]=(char)('0'+v/100); p[1]=(char)('0'+v/10%10); p[2]=(char)('0'+v%10); }

static void mk(void)
{
    FILE *f = fopen(nm, "wb"); if (f) { fwrite("12345", 1, 5, f); fclose(f); }
}
TC_CASE(initial)
{
    long p; FILE *f; mk();
    f = fopen(nm, "ab"); CHECK(f != NULL); if (!f) return;
    p = ftell(f);
    CHECK(p == 0 || p == 5);
    fclose(f);
}
TC_CASE(after_write)
{
    FILE *f; mk();
    f = fopen(nm, "ab"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite("abc", 1, 3, f), 3);
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_I(ftell(f), 8);
    fclose(f);
}
int main(void) { tc_begin(); setnm(); TC_RUN(initial); TC_RUN(after_write); remove(nm); return tc_end(); }
