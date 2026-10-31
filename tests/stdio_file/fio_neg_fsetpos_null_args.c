/* TC
 * id:    fio_neg_fsetpos_null_args
 * area:  stdio_file
 * funcs: fsetpos fopen fclose fwrite
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fsetpos (parameter validation, EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *n, const char *d, size_t len)
{
    FILE *f = fopen(n, "wb");
    if (f) { fwrite(d, 1, len, f); fclose(f); }
}
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_fio_neg_fsetpos_null_args_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(null_stream)
{
    fpos_t pos = 0; volatile int rc; FILE *volatile p = NULL;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    rc = fsetpos(p, &pos);
    CHECK(rc != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_pos)
{
    FILE *fp; volatile int rc; const fpos_t *volatile pp = NULL;
    mk(fn, "abc", 3);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    rc = fsetpos(fp, pp);
    CHECK(rc != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(null_stream); TC_RUN(null_pos); return tc_end(); }
