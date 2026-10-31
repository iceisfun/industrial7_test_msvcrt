/* TC
 * id:    chr_ungetc_buffer_boundary
 * area:  stdio_char
 * funcs: ungetc fgetc fread
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10 (one pushback guaranteed after any read, including just after a buffer refill)
 */
#include "tc.h"
#include <stdio.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_ungetc_buffer_boundary_a" TCV_S(TC_CRT_VER) ".tmp"
#define N 20000
static unsigned char data[N], tail[N];

static FILE *mk(void)
{
    FILE *f; int i;
    for (i = 0; i < N; i++) data[i] = (unsigned char)((i * 7 + i / 251) & 0xff);
    f = fopen(FN, "wb"); if (f) { fwrite(data, 1, N, f); fclose(f); }
    return fopen(FN, "rb");
}
static void readn(FILE *f, int n) { int i; for (i = 0; i < n; i++) if (fgetc(f) != data[i]) { CHECK(0 && "prefix mismatch"); return; } }

TC_CASE(after_4096_plus_1)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    readn(f, 4097);
    CHECK_EQ_I(ungetc(data[4096], f), data[4096]);
    CHECK_EQ_I(fgetc(f), data[4096]);
    CHECK_EQ_I(fgetc(f), data[4097]);
    fclose(f); remove(FN);
}
TC_CASE(after_512)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    readn(f, 512);
    CHECK_EQ_I(ungetc(data[511], f), data[511]);
    CHECK_EQ_I(fgetc(f), data[511]);
    CHECK_EQ_I(fgetc(f), data[512]);
    fclose(f); remove(FN);
}
TC_CASE(after_8193_tail)
{
    size_t got; FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    readn(f, 8193);
    CHECK_EQ_I(ungetc(data[8192], f), data[8192]);
    got = fread(tail, 1, N, f);
    CHECK_EQ_U(got, N - 8192);
    CHECK_MEM(tail, data + 8192, N - 8192);
    fclose(f); remove(FN);
}
TC_CASE(exactly_at_4096)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    readn(f, 4096);
    CHECK_EQ_I(ungetc(data[4095], f), data[4095]);
    CHECK_EQ_I(fgetc(f), data[4095]);
    CHECK_EQ_I(fgetc(f), data[4096]);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(after_4096_plus_1); TC_RUN(after_512); TC_RUN(after_8193_tail); TC_RUN(exactly_at_4096); return tc_end(); }
