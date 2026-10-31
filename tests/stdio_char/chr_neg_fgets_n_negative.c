/* TC
 * id:    chr_neg_fgets_n_negative
 * area:  stdio_char
 * funcs: fgets
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fgets; C11 7.21.7.2 (n<=0 not defined by the standard)
 * note:  PROBE: fgets with n<0. negative_count asserts only NULL return, untouched buffer, unconsumed input (a CRT that\n *        runs the read loop on negative n would fail it). negative_count_handler guesses the invalid-parameter handler\n *        runs with EINVAL; older CRTs may silently return NULL instead (n<=0 early return).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *name, const char *data, size_t n)
{
    FILE *f = fopen(name, "wb");
    if (f) { if (n) fwrite(data, 1, n, f); fclose(f); }
}

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_chr_neg_fgets_n_negative_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(negative_count)
{
    FILE *f; char buf[8]; char *r; int i, same = 1; volatile int n = -1;
    tc_iph_install();
    mk(FN, "xyz\n", 4);
    f = fopen(FN, "rb");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    for (i = 0; i < 8; i++) buf[i] = 'Q';
    r = fgets(buf, n, f);
    CHECK(r == NULL);
    for (i = 0; i < 8; i++) if (buf[i] != 'Q') same = 0;
    CHECK(same);
    CHECK_EQ_I(fgetc(f), 'x');
    fclose(f); remove(FN);
}
TC_CASE(negative_count_handler)
{
    FILE *f; char buf[8]; volatile int n = -1;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    mk(FN, "xyz\n", 4);
    f = fopen(FN, "rb");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    tc_iph_count = 0; errno = 0;
    (void)fgets(buf, n, f);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(negative_count); TC_RUN(negative_count_handler); return tc_end(); }
