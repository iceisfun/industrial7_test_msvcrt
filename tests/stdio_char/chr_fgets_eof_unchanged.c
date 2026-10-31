/* TC
 * id:    chr_fgets_eof_unchanged
 * area:  stdio_char
 * funcs: fgets
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.2p3 (EOF with no characters read: array contents unchanged, returns null pointer)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_fgets_eof_unchanged_a" TCV_S(TC_CRT_VER) ".tmp"

TC_CASE(empty_file)
{
    char buf[8]; FILE *f = fopen(FN, "wb"); if (f) fclose(f);
    memset(buf, 'Q', sizeof buf);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(buf, 8, f) == NULL);
    CHECK_MEM(buf, "QQQQQQQQ", 8);
    CHECK(feof(f) != 0); CHECK_EQ_I(ferror(f), 0);
    fclose(f); remove(FN);
}
TC_CASE(after_last_line)
{
    char buf[8]; FILE *f = fopen(FN, "wb"); if (f) { fputs("one\n", f); fclose(f); }
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(buf, 8, f) == buf); CHECK_STR(buf, "one\n");
    memset(buf, 'Q', sizeof buf);
    CHECK(fgets(buf, 8, f) == NULL);
    CHECK_MEM(buf, "QQQQQQQQ", 8);
    CHECK(feof(f) != 0); CHECK_EQ_I(ferror(f), 0);
    CHECK(fgets(buf, 8, f) == NULL);     /* stays at EOF */
    fclose(f); remove(FN);
}
TC_CASE(text_mode_empty)
{
    char buf[8]; FILE *f = fopen(FN, "wb"); if (f) fclose(f);
    memset(buf, 'Q', sizeof buf);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(buf, 8, f) == NULL);
    CHECK_MEM(buf, "QQQQQQQQ", 8);
    CHECK(feof(f) != 0);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(empty_file); TC_RUN(after_last_line); TC_RUN(text_mode_empty); return tc_end(); }
