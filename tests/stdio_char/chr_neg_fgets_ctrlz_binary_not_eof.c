/* TC
 * id:    chr_neg_fgets_ctrlz_binary_not_eof
 * area:  stdio_char
 * funcs: fgets fopen fclose fwrite feof
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fopen (t/b modes: Ctrl+Z is an end-of-file marker only in text mode)
 * note:  Binary mode treats 0x1A as ordinary data, so fgets must not stop at it. Contrast case: text mode treats it as EOF.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_fgets_ctrlz_binary_not_eof_" TCS(TC_CRT_VER) ".tmp"

static void mk(void)
{
    FILE *f = fopen(FN, "wb");
    if (f) { fwrite("a\x1a" "b\n", 1, 4, f); fclose(f); }
}
TC_CASE(binary_reads_through)
{
    FILE *f; char buf[16] = {0}; char *p;
    mk(); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    p = fgets(buf, (int)sizeof buf, f);
    CHECK(p == buf);
    CHECK_EQ_U(strlen(buf), 4);
    CHECK_MEM(buf, "a\x1a" "b\n", 4);
    CHECK_EQ_I(feof(f), 0);
    fclose(f); remove(FN);
}
TC_CASE(text_stops_at_ctrlz)
{
    FILE *f; char buf[16] = {0}; char *p;
    mk(); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    p = fgets(buf, (int)sizeof buf, f);
    CHECK(p == buf);
    CHECK_STR(buf, "a");
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(binary_reads_through); TC_RUN(text_stops_at_ctrlz); return tc_end(); }
