/* TC
 * id:    chr_ungetc_basic
 * area:  stdio_char
 * funcs: ungetc fgetc
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10 (one character of pushback is guaranteed)
 */
#include "tc.h"
#include <stdio.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_ungetc_basic_a" TCV_S(TC_CRT_VER) ".tmp"
static FILE *mk(const char *d, size_t n)
{
    FILE *f = fopen(FN, "wb"); if (f) { fwrite(d, 1, n, f); fclose(f); }
    return fopen(FN, "rb");
}

TC_CASE(same_char)
{
    FILE *f = mk("abcd", 4); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(ungetc('a', f), 'a');
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(fgetc(f), 'b');
    fclose(f); remove(FN);
}
TC_CASE(different_char)
{
    FILE *f = mk("abcd", 4); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b');
    CHECK_EQ_I(ungetc('Z', f), 'Z');
    CHECK_EQ_I(fgetc(f), 'Z');
    CHECK_EQ_I(fgetc(f), 'c');
    fclose(f); remove(FN);
}
TC_CASE(then_eof)
{
    FILE *f = mk("ab", 2); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b');
    CHECK_EQ_I(ungetc('b', f), 'b');
    CHECK_EQ_I(fgetc(f), 'b');
    CHECK_EQ_I(fgetc(f), EOF);
    fclose(f); remove(FN);
}
TC_CASE(before_eof_seen)
{
    FILE *f = mk("xy", 2); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'x'); CHECK_EQ_I(fgetc(f), 'y');
    CHECK_EQ_I(ungetc('q', f), 'q');
    CHECK_EQ_I(fgetc(f), 'q');
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK_EQ_I(fgetc(f), EOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(same_char); TC_RUN(different_char); TC_RUN(then_eof); TC_RUN(before_eof_seen); return tc_end(); }
