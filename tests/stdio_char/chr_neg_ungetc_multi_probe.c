/* TC
 * id:    chr_neg_ungetc_multi_probe
 * area:  stdio_char
 * kind:  negative
 * funcs: ungetc fgetc setvbuf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.7.10p3 (only one pushback guaranteed); MS CRT behaviour not documented
 * note:  Probes: (1) on a buffered stream after reading 3 bytes, two consecutive ungetc succeed and read back LIFO (the CRT just steps
 *        the buffer pointer back); (2) on an _IONBF stream a second ungetc without an intervening read fails with EOF.
 */
#include "tc.h"
#include <stdio.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_ungetc_multi_probe_a" TCV_S(TC_CRT_VER) ".tmp"
static FILE *mk(int nbf)
{
    FILE *f = fopen(FN, "wb"); if (f) { fputs("abcdef", f); fclose(f); }
    f = fopen(FN, "rb");
    if (f && nbf) CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    return f;
}

TC_CASE(buffered_two)
{
    FILE *f = mk(0); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), 'c');
    CHECK_EQ_I(ungetc('X', f), 'X');
    CHECK_EQ_I(ungetc('Y', f), 'Y');
    CHECK_EQ_I(fgetc(f), 'Y'); CHECK_EQ_I(fgetc(f), 'X'); CHECK_EQ_I(fgetc(f), 'd');   /* c was consumed before the pushbacks */
    fclose(f); remove(FN);
}
TC_CASE(unbuffered_second_fails)
{
    FILE *f = mk(1); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(ungetc('X', f), 'X');
    CHECK_EQ_I(ungetc('Y', f), EOF);
    CHECK_EQ_I(fgetc(f), 'X');
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(buffered_two); TC_RUN(unbuffered_second_fails); return tc_end(); }
