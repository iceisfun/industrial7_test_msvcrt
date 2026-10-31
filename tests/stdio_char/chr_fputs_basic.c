/* TC
 * id:    chr_fputs_basic
 * area:  stdio_char
 * funcs: fputs
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.4 (writes string without terminating NUL; no newline appended; nonnegative on success)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_fputs_basic_a" TCV_S(TC_CRT_VER) ".tmp"
static size_t slurp(unsigned char *buf, size_t max)
{
    FILE *f = fopen(FN, "rb"); size_t n = 0;
    if (f) { n = fread(buf, 1, max, f); fclose(f); }
    return n;
}

TC_CASE(no_newline_added)
{
    unsigned char b[32]; int r; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    r = fputs("abc", f); CHECK(r >= 0);
    r = fputs("def", f); CHECK(r >= 0);
    fclose(f);
    CHECK_EQ_U(slurp(b, 32), 6); CHECK_MEM(b, "abcdef", 6);
    remove(FN);
}
TC_CASE(empty_string)
{
    unsigned char b[32]; int r; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    r = fputs("", f); CHECK(r >= 0);
    fclose(f);
    CHECK_EQ_U(slurp(b, 32), 0);
    remove(FN);
}
TC_CASE(stops_at_nul)
{
    static const char s[] = "ab\0cd";
    unsigned char b[32]; int r; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    r = fputs(s, f); CHECK(r >= 0);
    fclose(f);
    CHECK_EQ_U(slurp(b, 32), 2); CHECK_MEM(b, "ab", 2);
    remove(FN);
}
TC_CASE(trailing_nul_not_written)
{
    unsigned char b[32]; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fputs("x", f); fputs("y", f);
    fclose(f);
    CHECK_EQ_U(slurp(b, 32), 2); CHECK_MEM(b, "xy", 2);
    remove(FN);
}
int main(void) { tc_begin(); TC_RUN(no_newline_added); TC_RUN(empty_string); TC_RUN(stops_at_nul); TC_RUN(trailing_nul_not_written); return tc_end(); }
