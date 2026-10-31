/* TC
 * id:    chr_fgets_embedded_nul
 * area:  stdio_char
 * funcs: fgets
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.2 (reads characters, stores NUL after last)
 * note:  NUL bytes are read like any other byte; only the string view is truncated.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_fgets_embedded_nul_a" TCV_S(TC_CRT_VER) ".tmp"
static void mk(const char *d, size_t n) { FILE *f = fopen(FN, "wb"); fwrite(d, 1, n, f); fclose(f); }

TC_CASE(nul_inside)
{
    static const char d[] = "ab\0cd\n";
    static const unsigned char want[] = { 'a', 'b', 0, 'c', 'd', '\n', 0, 0x55 };
    unsigned char buf[16]; FILE *f; mk(d, 6);
    memset(buf, 0x55, sizeof buf);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets((char *)buf, 16, f) == (char *)buf);
    CHECK_MEM(buf, want, 8);
    CHECK_EQ_U(strlen((char *)buf), 2);
    CHECK(fgets((char *)buf, 16, f) == NULL);   /* whole line consumed */
    fclose(f); remove(FN);
}
TC_CASE(nul_first)
{
    static const char d[] = "\0xy\n";
    unsigned char buf[16]; FILE *f; mk(d, 4);
    memset(buf, 0x55, sizeof buf);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets((char *)buf, 16, f) == (char *)buf);
    CHECK_EQ_U(buf[0], 0); CHECK_EQ_U(buf[1], 'x'); CHECK_EQ_U(buf[2], 'y'); CHECK_EQ_U(buf[3], '\n'); CHECK_EQ_U(buf[4], 0);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(nul_inside); TC_RUN(nul_first); return tc_end(); }
