/* TC
 * id:    fio_neg_fseek_negative_pos
 * area:  stdio_file
 * kind:  negative
 * funcs: fseek fopen ftell fread fclose
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fseek; _lseek (negative resulting offset -> EINVAL)
 * note:  iph installed where available only so a UCRT fatal handler cannot kill the run; its count is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char nm[] = "tc_fio_neg_fseek_negative_pos_000.tmp";  /* per-CRT suffix: targets may run concurrently */
static void setnm(void) { int v = TC_CRT_VER; char *p = nm + sizeof nm - 9; p[0]=(char)('0'+v/100); p[1]=(char)('0'+v/10%10); p[2]=(char)('0'+v%10); }

static FILE *mk10(void)
{
    FILE *f = fopen(nm, "wb");
    if (f) { fwrite("0123456789", 1, 10, f); fclose(f); }
    return fopen(nm, "rb");
}
TC_CASE(set_neg)
{
    FILE *f = mk10(); CHECK(f != NULL); if (!f) return;
    errno = 0; CHECK(fseek(f, -1, SEEK_SET) != 0); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(ftell(f), 0);
    fclose(f);
}
TC_CASE(cur_neg)
{
    char b[8]; FILE *f = mk10(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(b, 1, 5, f), 5);
    errno = 0; CHECK(fseek(f, -100, SEEK_CUR) != 0); CHECK_EQ_I(errno, EINVAL);
    fclose(f);
}
TC_CASE(end_neg)
{
    FILE *f = mk10(); CHECK(f != NULL); if (!f) return;
    errno = 0; CHECK(fseek(f, -100, SEEK_END) != 0); CHECK_EQ_I(errno, EINVAL);
    fclose(f);
}
int main(void) { tc_begin(); setnm(); tc_iph_install(); TC_RUN(set_neg); TC_RUN(cur_neg); TC_RUN(end_neg); remove(nm); return tc_end(); }
