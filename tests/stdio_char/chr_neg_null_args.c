/* TC
 * id:    chr_neg_null_args
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fputc fgets fputs ungetc
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation", fgets/fputs/fgetc/fputc/ungetc (null pointer: invalid parameter handler, EINVAL, EOF/NULL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_null_args_a" TCV_S(TC_CRT_VER) ".tmp"
static FILE *volatile nf;
static char *volatile nb;

TC_CASE(null_stream)
{
    char b[8]; int r;
    nf = NULL;
    errno = 0; tc_iph_count = 0; r = fgetc(nf);          CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    errno = 0; tc_iph_count = 0; r = fputc('a', nf);     CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    errno = 0; tc_iph_count = 0; r = fputs("a", nf);     CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    errno = 0; tc_iph_count = 0; r = ungetc('a', nf);    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    errno = 0; tc_iph_count = 0; CHECK(fgets(b, 8, nf) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer)
{
    FILE *f = fopen(FN, "wb"); int r; CHECK(f != NULL); if (!f) return;
    nb = NULL; fclose(f); f = fopen(FN, "rb"); if (!f) return;
    errno = 0; tc_iph_count = 0; CHECK(fgets(nb, 8, f) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
    f = fopen(FN, "wb"); if (!f) return;
    errno = 0; tc_iph_count = 0; r = fputs(nb, f); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_buffer); TC_RUN(null_stream);
    return tc_end();
}
