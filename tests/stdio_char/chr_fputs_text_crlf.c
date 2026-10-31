/* TC
 * id:    chr_fputs_text_crlf
 * area:  stdio_char
 * funcs: fputs
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode: LF translated to CR-LF on output)
 * note:  The third case (an embedded CR LF becomes CR CR LF) follows from the LF->CRLF rule applied per byte.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_fputs_text_crlf_a" TCV_S(TC_CRT_VER) ".tmp"
static size_t slurp(unsigned char *buf, size_t max)
{
    FILE *f = fopen(FN, "rb"); size_t n = 0;
    if (f) { n = fread(buf, 1, max, f); fclose(f); }
    return n;
}

TC_CASE(text_lf_to_crlf)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    fputs("a\nb\n", f); fclose(f);
    CHECK_EQ_U(slurp(b, 32), 6); CHECK_MEM(b, "a\r\nb\r\n", 6);
    remove(FN);
}
TC_CASE(binary_lf_only)
{
    unsigned char b[32]; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fputs("a\nb\n", f); fclose(f);
    CHECK_EQ_U(slurp(b, 32), 4); CHECK_MEM(b, "a\nb\n", 4);
    remove(FN);
}
TC_CASE(text_embedded_crlf)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    fputs("x\r\ny", f); fclose(f);
    CHECK_EQ_U(slurp(b, 32), 5); CHECK_MEM(b, "x\r\r\ny", 5);
    remove(FN);
}
int main(void) { tc_begin(); TC_RUN(text_lf_to_crlf); TC_RUN(binary_lf_only); TC_RUN(text_embedded_crlf); return tc_end(); }
