/* TC
 * id:    chr_fgets_ctrlz_text
 * area:  stdio_char
 * funcs: fgets
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode: Ctrl+Z is interpreted as end-of-file on input)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_fgets_ctrlz_text_a" TCV_S(TC_CRT_VER) ".tmp"
static void mk(const char *d, size_t n) { FILE *f = fopen(FN, "wb"); fwrite(d, 1, n, f); fclose(f); }

TC_CASE(after_line)
{
    char b[32]; FILE *f; mk("ab\n\x1a" " cd", 7);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 32, f) == b); CHECK_STR(b, "ab\n");
    CHECK(fgets(b, 32, f) == NULL);
    CHECK(feof(f) != 0);
    fclose(f); remove(FN);
}
TC_CASE(mid_line)
{
    char b[32]; FILE *f; mk("ab\x1a", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 32, f) == b); CHECK_STR(b, "ab");
    CHECK(fgets(b, 32, f) == NULL);
    fclose(f); remove(FN);
}
TC_CASE(binary_reads_past)
{
    char b[32]; FILE *f; mk("ab\n\x1a" " cd", 7);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 32, f) == b); CHECK_STR(b, "ab\n");
    CHECK(fgets(b, 32, f) == b); CHECK_STR(b, "\x1a" " cd");
    CHECK(fgets(b, 32, f) == NULL);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(after_line); TC_RUN(mid_line); TC_RUN(binary_reads_past); return tc_end(); }
