/* TC
 * id:    chr_neg_fgets_n_zero
 * area:  stdio_char
 * funcs: fgets
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.7.2 (n==0 unspecified); MS docs fgets
 * note:  PROBE: fgets with n==0. null_untouched asserts only NULL return, untouched buffer, unconsumed input.
 *        silent_probe guesses no invalid-parameter call is made and errno is unchanged.
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
#define FN "tc_chr_neg_fgets_n_zero_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(null_untouched)
{
    FILE *f; char buf[4]; char *r; int i, same = 1; volatile int n = 0;
    tc_iph_install();
    mk(FN, "xyz\n", 4);
    f = fopen(FN, "rb");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    for (i = 0; i < 4; i++) buf[i] = 'Q';
    r = fgets(buf, n, f);
    CHECK(r == NULL);
    for (i = 0; i < 4; i++) if (buf[i] != 'Q') same = 0;
    CHECK(same);
    CHECK_EQ_I(fgetc(f), 'x');
    fclose(f); remove(FN);
}
TC_CASE(silent_probe)
{
    FILE *f; char buf[4]; volatile int n = 0;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    mk(FN, "xyz\n", 4);
    f = fopen(FN, "rb");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    tc_iph_count = 0; errno = 0;
    (void)fgets(buf, n, f);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(errno, 0);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(null_untouched); TC_RUN(silent_probe); return tc_end(); }
