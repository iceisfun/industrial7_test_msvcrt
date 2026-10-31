/* TC
 * id:    chr_ungetc_unbuffered
 * area:  stdio_char
 * funcs: ungetc fgetc fgets setvbuf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.6 (setvbuf before other operations), 7.21.7.10
 */
#include "tc.h"
#include <stdio.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_ungetc_unbuffered_a" TCV_S(TC_CRT_VER) ".tmp"
static FILE *mk(void)
{
    FILE *f = fopen(FN, "wb"); if (f) { fputs("abc\n", f); fclose(f); }
    f = fopen(FN, "rb");
    if (f) CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    return f;
}

TC_CASE(same_char)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(ungetc('a', f), 'a');
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(fgetc(f), 'b');
    fclose(f); remove(FN);
}
TC_CASE(different_char)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(ungetc('Z', f), 'Z');
    CHECK_EQ_I(fgetc(f), 'Z');
    CHECK_EQ_I(fgetc(f), 'b');
    fclose(f); remove(FN);
}
TC_CASE(then_fgets)
{
    char b[16]; FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(ungetc('Z', f), 'Z');
    CHECK(fgets(b, 16, f) == b);
    CHECK_STR(b, "Zbc\n");
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(same_char); TC_RUN(different_char); TC_RUN(then_fgets); return tc_end(); }
