/* TC
 * id:    chr_ungetc_position
 * area:  stdio_char
 * funcs: ungetc fgetc
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10p5 (binary stream: file position decremented by a successful ungetc)
 * note:  fseek/rewind discarding pushback moved to chr_probe_ungetc_fseek_discards (real CRTs may satisfy it from the stdio buffer).
 */
#include "tc.h"
#include <stdio.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_ungetc_position_a" TCV_S(TC_CRT_VER) ".tmp"
static FILE *mk(void)
{
    FILE *f = fopen(FN, "wb"); if (f) { fwrite("abcdef", 1, 6, f); fclose(f); }
    return fopen(FN, "rb");
}

TC_CASE(ftell_adjusts)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), 'c');
    CHECK_EQ_I(ftell(f), 3);
    CHECK_EQ_I(ungetc('c', f), 'c');
    CHECK_EQ_I(ftell(f), 2);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(ftell_adjusts); return tc_end(); }
