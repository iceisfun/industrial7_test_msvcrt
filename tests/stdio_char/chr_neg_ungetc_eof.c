/* TC
 * id:    chr_neg_ungetc_eof
 * area:  stdio_char
 * kind:  negative
 * funcs: ungetc fgetc
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10p4 (if c equals EOF the operation fails and the stream is unchanged); MS docs ungetc
 */
#include "tc.h"
#include <stdio.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_ungetc_eof_a" TCV_S(TC_CRT_VER) ".tmp"
static volatile int veof = EOF;

TC_CASE(push_eof_fails)
{
    FILE *f = fopen(FN, "wb"); if (f) { fputs("abc", f); fclose(f); }
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(ungetc(veof, f), EOF);
    CHECK_EQ_I(fgetc(f), 'b');        /* position and stream unchanged */
    CHECK_EQ_I(ferror(f), 0);
    fclose(f); remove(FN);
}
TC_CASE(eof_indicator_not_cleared)
{
    FILE *f = fopen(FN, "wb"); if (f) { fputs("a", f); fclose(f); }
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), EOF); CHECK(feof(f) != 0);
    CHECK_EQ_I(ungetc(veof, f), EOF);
    CHECK(feof(f) != 0);              /* failed ungetc leaves the EOF indicator set */
    CHECK_EQ_I(fgetc(f), EOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(push_eof_fails); TC_RUN(eof_indicator_not_cleared); return tc_end(); }
