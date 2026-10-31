/* TC
 * id:    chr_neg_fputs_null_string
 * area:  stdio_char
 * funcs: fputs
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fputs, Parameter validation
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
#define FN "tc_chr_neg_fputs_null_string_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(null_string)
{
    FILE *f; int r; volatile const char *ns = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    f = fopen(FN, "wb");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    tc_iph_count = 0; errno = 0;
    r = fputs((const char *)ns, f);
    CHECK_EQ_I(r, EOF);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
    f = fopen(FN, "rb");
    CHECK(f != NULL);
    if (f) { CHECK_EQ_I(fgetc(f), EOF); fclose(f); }
    remove(FN);
}
int main(void) { tc_begin(); TC_RUN(null_string); return tc_end(); }
