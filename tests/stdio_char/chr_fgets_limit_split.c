/* TC
 * id:    chr_fgets_limit_split
 * area:  stdio_char
 * funcs: fgets
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.2; MS docs fgets
 * note:  Long lines are split across several fgets calls; sizes chosen to cross the 4096/8192 internal buffer boundaries.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_fgets_limit_split_a" TCV_S(TC_CRT_VER) ".tmp"
static char data[9000], out[9000];

static void mk(size_t n)
{
    FILE *f = fopen(FN, "wb"); size_t i;
    for (i = 0; i + 1 < n; i++) data[i] = (char)('a' + i % 26);
    data[n - 1] = '\n';
    fwrite(data, 1, n, f); fclose(f);
}

TC_CASE(chunks_100)
{
    char buf[100]; int chunks = 0, nl_early = 0; size_t tot = 0;
    FILE *f; mk(5001);                 /* 5000 chars + LF */
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    while (fgets(buf, 100, f)) {
        size_t l = strlen(buf);
        if (l && buf[l - 1] == '\n') { if (tot + l != 5001) nl_early = 1; }
        memcpy(out + tot, buf, l); tot += l; chunks++;
    }
    CHECK_EQ_I(chunks, 51);            /* 50 full chunks of 99 + final 51 bytes incl. LF */
    CHECK_EQ_I(nl_early, 0);
    CHECK_EQ_U(tot, 5001);
    CHECK_MEM(out, data, 5001);
    fclose(f); remove(FN);
}
TC_CASE(cross_4096_8192)
{
    char buf[4097]; int chunks = 0; size_t tot = 0, l; FILE *f; mk(8200);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    while (fgets(buf, 4097, f)) { l = strlen(buf); memcpy(out + tot, buf, l); tot += l; chunks++; }
    CHECK_EQ_I(chunks, 3);             /* 4096 + 4096 + 8 (7 chars + LF) */
    CHECK_EQ_U(tot, 8200);
    CHECK_MEM(out, data, 8200);
    fclose(f); remove(FN);
}
TC_CASE(exactly_n_minus_1)
{
    char buf[10]; FILE *f; mk(10);     /* 9 chars + LF, n = 10 */
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(buf, 10, f) == buf);
    CHECK_EQ_U(strlen(buf), 9);
    CHECK_MEM(buf, data, 9);
    CHECK(fgets(buf, 10, f) == buf);
    CHECK_STR(buf, "\n");
    CHECK(fgets(buf, 10, f) == NULL);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(chunks_100); TC_RUN(cross_4096_8192); TC_RUN(exactly_n_minus_1); return tc_end(); }
