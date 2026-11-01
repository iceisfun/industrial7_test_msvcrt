/* TC
 * id:    chr_neg_fgets_n_probe
 * area:  stdio_char
 * kind:  negative
 * funcs: fgets
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.7.2 (n==1: stores only NUL); MS fgets validates n > 0 (not stated in the docs for n)
 * note:  Probes (1) fgets with n == 0 and n < 0 returns NULL, sets errno EINVAL, invokes the invalid-parameter handler and does not
 *        touch the buffer; (2) fgets with n == 1 returns buf with buf[0] == 0 and consumes nothing.
 *        TRIAGE win1093[34])?#934, class A): on ucrtbase 10.0.19045 fgets(buf, 0, f) returns NULL with errno 0 and NO handler call (only n < 0 is validated);
 *        buffer untouched and nothing consumed, as asserted. EINVAL+handler kept for non-UCRT targets (unverified).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_fgets_n_probe_a" TCV_S(TC_CRT_VER) ".tmp"
static FILE *mk(void)
{
    FILE *f = fopen(FN, "wb"); if (f) { fputs("abc\n", f); fclose(f); }
    return fopen(FN, "rb");
}
static volatile int vn;

TC_CASE(n_zero)
{
    char b[16] = { 'Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q' }; FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    vn = 0; errno = 0; tc_iph_count = 0;
    CHECK(fgets(b, vn, f) == NULL);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);   /* observed ucrtbase 10.0.19045: n == 0 returns NULL silently */
#else
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
#endif
    CHECK_MEM(b, "QQQQQQQQQQQQQQQQ", 16);
    CHECK_EQ_I(fgetc(f), 'a');
    fclose(f); remove(FN);
}
TC_CASE(n_negative)
{
    char b[16] = { 'Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q' }; FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    vn = -1; errno = 0; tc_iph_count = 0;
    CHECK(fgets(b, vn, f) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_MEM(b, "QQQQQQQQQQQQQQQQ", 16);
    fclose(f); remove(FN);
}
TC_CASE(n_one)
{
    char b[16] = { 'Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q','Q' }; FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    vn = 1;
    CHECK(fgets(b, vn, f) == b); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'Q');
    CHECK_EQ_I(fgetc(f), 'a');
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(n_zero); TC_RUN(n_negative); TC_RUN(n_one); return tc_end(); }
