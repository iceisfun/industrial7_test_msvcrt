/* TC
 * id:    chr_ungetc_clears_eof
 * area:  stdio_char
 * funcs: ungetc fgetc
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10p5 (a successful ungetc clears the end-of-file indicator)
 */
#include "tc.h"
#include <stdio.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_ungetc_clears_eof_a" TCV_S(TC_CRT_VER) ".tmp"

TC_CASE(clears)
{
    FILE *f = fopen(FN, "wb"); if (f) { fputs("a", f); fclose(f); }
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(ungetc('x', f), 'x');
    CHECK_EQ_I(feof(f), 0);
    CHECK_EQ_I(fgetc(f), 'x');
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(ferror(f), 0);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(clears); return tc_end(); }
