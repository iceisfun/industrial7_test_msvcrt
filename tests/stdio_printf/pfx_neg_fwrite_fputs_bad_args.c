/* TC
 * id:    pfx_neg_fwrite_fputs_bad_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwrite fputs fopen fclose
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fputs (NULL str or stream -> EOF, EINVAL), fwrite (NULL buffer or stream -> 0, EINVAL; zero size or count returns 0)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile path = "tc_pfx_neg_fwrite_a.tmp";
static const char *volatile nstr = NULL;
static const void *volatile nbuf = NULL;
static FILE *volatile nf = NULL;
static volatile size_t zero = 0, four = 4;
static char data[8] = "abcdefg";

TC_CASE(fputs_null_str)
{
    FILE *f = fopen(path, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fputs(nstr, f);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
TC_CASE(fputs_null_stream)
{
    int r; tc_iph_count = 0; errno = 0; r = fputs("x", nf);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fwrite_null_buf)
{
    FILE *f = fopen(path, "w"); size_t r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fwrite(nbuf, 1, four, f);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
TC_CASE(fwrite_zero_size_count)
{
    FILE *f = fopen(path, "w"); size_t r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    r = fwrite(data, zero, four, f); CHECK_EQ_U(r, 0);
    r = fwrite(data, 4, zero, f); CHECK_EQ_U(r, 0);
    CHECK_EQ_I(tc_iph_count, 0);
    fclose(f);
}
TC_CASE(fwrite_null_stream)
{
    size_t r; tc_iph_count = 0; errno = 0; r = fwrite(data, 1, four, nf);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(fputs_null_str); TC_RUN(fputs_null_stream); TC_RUN(fwrite_null_buf); TC_RUN(fwrite_zero_size_count); TC_RUN(fwrite_null_stream);
    DeleteFileA(path);
    return tc_end();
}
