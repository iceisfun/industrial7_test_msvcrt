/* TC
 * id:    fio_neg_fseeki64_args
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: _fseeki64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fseeki64
 * note:  
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, _fseeki64, (FILE *, long long, int));
static FILE * volatile nf = NULL;
static volatile int w7 = 7;
static char nm[] = "tc_fio_neg_fseeki64_args_000.tmp";  /* per-CRT suffix: targets may run concurrently */
static void setnm(void) { int v = TC_CRT_VER; char *p = nm + sizeof nm - 9; p[0]=(char)('0'+v/100); p[1]=(char)('0'+v/10%10); p[2]=(char)('0'+v%10); }

TC_CASE(null_stream)
{
    int before = tc_iph_count;
    if (!tc_p__fseeki64) TC_SKIP("not exported");
    errno = 0;
    CHECK_EQ_I(tc_p__fseeki64(nf, 0, SEEK_SET), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(bad_whence)
{
    FILE *f; int before = tc_iph_count;
    if (!tc_p__fseeki64) TC_SKIP("not exported");
    f = fopen(nm, "w+b"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    CHECK_EQ_I(tc_p__fseeki64(f, 0, w7), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
    fclose(f);
}
TC_CASE(negative_result)
{
    FILE *f;
    if (!tc_p__fseeki64) TC_SKIP("not exported");
    f = fopen(nm, "w+b"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    CHECK(tc_p__fseeki64(f, -1, SEEK_SET) != 0);
    CHECK_EQ_I(errno, EINVAL);
    fclose(f);
}
int main(void)
{
    tc_begin(); setnm(); tc_iph_install(); TC_LOAD_PROC(_fseeki64);
    TC_RUN(null_stream); TC_RUN(bad_whence); TC_RUN(negative_result);
    remove(nm); return tc_end();
}
