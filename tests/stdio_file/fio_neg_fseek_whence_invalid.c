/* TC
 * id:    fio_neg_fseek_whence_invalid
 * area:  stdio_file
 * kind:  negative
 * funcs: fseek fopen fread ftell fclose
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fseek (invalid origin -> invalid parameter handler, EINVAL, returns -1)
 * note:  
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char nm[] = "tc_fio_neg_fseek_whence_invalid_000.tmp";  /* per-CRT suffix: targets may run concurrently */
static void setnm(void) { int v = TC_CRT_VER; char *p = nm + sizeof nm - 9; p[0]=(char)('0'+v/100); p[1]=(char)('0'+v/10%10); p[2]=(char)('0'+v%10); }
static volatile int w3 = 3, wm1 = -1, w99 = 99;

TC_CASE(bad_whence)
{
    char b[8]; int before; FILE *f = fopen(nm, "wb");
    CHECK(f != NULL); if (!f) return;
    fwrite("0123456789", 1, 10, f); fclose(f);
    f = fopen(nm, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(b, 1, 4, f), 4);
    before = tc_iph_count;
    errno = 0; CHECK_EQ_I(fseek(f, 0, w3), -1);  CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, before + 1);
    errno = 0; CHECK_EQ_I(fseek(f, 0, wm1), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, before + 2);
    errno = 0; CHECK_EQ_I(fseek(f, 0, w99), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, before + 3);
    CHECK_EQ_I(ftell(f), 4);
    fclose(f);
}
int main(void) { tc_begin(); setnm(); tc_iph_install(); TC_RUN(bad_whence); remove(nm); return tc_end(); }
