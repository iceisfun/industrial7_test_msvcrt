/* TC
 * id:    fio_neg_probe_fsetpos_negative
 * area:  stdio_file
 * funcs: fsetpos fopen fclose fwrite
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fsetpos; _lseeki64 rejects negative offsets with EINVAL
 * note:  PROBE: fsetpos with fpos_t -1 (a value fgetpos never yields). Expect nonzero return and errno EINVAL
 *        (underlying seek to a negative offset fails). No invalid-parameter handler assertion is made.
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
static const char *fn = "tc_fio_neg_probe_fsetpos_negative_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(negative)
{
    FILE *fp; fpos_t pos = -1; volatile int rc;
    mk(fn, "abc", 3);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    errno = 0;
    rc = fsetpos(fp, &pos);
    CHECK(rc != 0);
    CHECK_EQ_I(errno, EINVAL);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(negative); return tc_end(); }
