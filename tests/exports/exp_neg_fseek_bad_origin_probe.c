/* TC
 * id:    exp_neg_fseek_bad_origin_probe
 * area:  exports
 * funcs: fopen fseek fclose remove fwrite
 * procs:
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fseek; C11 7.21.9.2 (origin domain not specified further)
 * note:  kind=negative conf=probe. Probing: fseek with an invalid origin returns nonzero and sets EINVAL
 *        (ucrt may call the invalid-parameter handler; a handler is installed where available).
 */
#include "tc.h"
#include <errno.h>
#include <stdio.h>

static char fname[64] = "tc_exp_neg_fseek_bad_origin_probe_00000000.tmp";
static void uniq(void)
{
    DWORD pid = GetCurrentProcessId(); char *p = fname + 34; int i;
    for (i = 7; i >= 0; i--) { p[i] = "0123456789abcdef"[pid & 15]; pid >>= 4; }
}
static FILE *mk(void)
{
    FILE *f = fopen(fname, "wb+");
    if (f) fwrite("0123456789", 1, 10, f);
    return f;
}
TC_CASE(origin_99_nonzero)
{
    volatile int o = 99; FILE *f = mk(); int r;
    if (!f) { CHECK(f != NULL); return; }
    r = fseek(f, 0, o);
    CHECK(r != 0);
    fclose(f); remove(fname);
}
TC_CASE(origin_minus1_nonzero)
{
    volatile int o = -1; FILE *f = mk(); int r;
    if (!f) { CHECK(f != NULL); return; }
    r = fseek(f, 0, o);
    CHECK(r != 0);
    fclose(f); remove(fname);
}
TC_CASE(errno_einval)
{
    volatile int o = 99; FILE *f = mk(); int r;
    if (!f) { CHECK(f != NULL); return; }
    errno = 0;
    r = fseek(f, 0, o);
    CHECK(r != 0);
    CHECK_EQ_I(errno, EINVAL);
    fclose(f); remove(fname);
}
int main(void)
{
    tc_begin();
    uniq();
    remove(fname);
    if (TC_AT_LEAST(80)) tc_iph_install();
    TC_RUN(origin_99_nonzero); TC_RUN(origin_minus1_nonzero); TC_RUN(errno_einval);
    return tc_end();
}
