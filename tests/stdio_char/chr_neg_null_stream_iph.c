/* TC
 * id:    chr_neg_null_stream_iph
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fputc fgets fputs fgetwc fputwc ungetc _getw _putw
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter validation; fgetc/fputc/fputs: NULL stream invokes invalid-parameter handler, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
static FILE *volatile nf;
#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)

TC_CASE(fgetc_fputc)
{
    RESET(); CHECK_EQ_I(fgetc(nf), EOF); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
    RESET(); CHECK_EQ_I(fputc('a', nf), EOF); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(fgets_fputs)
{
    char b[8];
    RESET(); CHECK(fgets(b, 8, nf) == NULL); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
    RESET(); CHECK_EQ_I(fputs("x", nf), EOF); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(wide)
{
    RESET(); CHECK_EQ_U(fgetwc(nf), WEOF); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
    RESET(); CHECK_EQ_U(fputwc(L'a', nf), WEOF); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(ungetc_getw_putw)
{
    RESET(); CHECK_EQ_I(ungetc('a', nf), EOF); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
    RESET(); CHECK_EQ_I(_getw(nf), EOF); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
    RESET(); CHECK_EQ_I(_putw(1, nf), EOF); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(null_buffer)
{
    FILE *f;
    f = fopen("tc_chr_neg_null_stream_iph_" TC_S(TC_CRT_VER) ".tmp", "wb"); CHECK(f != NULL); if (!f) return;
    RESET(); CHECK_EQ_I(fputs(NULL, f), EOF); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
    RESET(); CHECK(fgets(NULL, 8, f) == NULL); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
    fclose(f); DeleteFileA("tc_chr_neg_null_stream_iph_" TC_S(TC_CRT_VER) ".tmp");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid-parameter handler API\n"); return tc_end(); }
    TC_RUN(fgetc_fputc); TC_RUN(fgets_fputs); TC_RUN(wide); TC_RUN(ungetc_getw_putw); TC_RUN(null_buffer);
    return tc_end();
}
