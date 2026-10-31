/* TC
 * id:    chr_neg_fgets_null_buffer
 * area:  stdio_char
 * funcs: fgets
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fgets, Parameter validation
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
#define FN "tc_chr_neg_fgets_null_buffer_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(null_buffer)
{
    FILE *f; char *r; volatile char *nb = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    mk(FN, "xyz\n", 4);
    f = fopen(FN, "rb");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    tc_iph_count = 0; errno = 0;
    r = fgets((char *)nb, 10, f);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(fgetc(f), 'x');
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(null_buffer); return tc_end(); }
