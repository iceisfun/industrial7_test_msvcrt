/* TC
 * id:    chr_neg_getw_partial_int
 * area:  stdio_char
 * funcs: _getw
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _getw (EOF on error or end of file)
 * note:  Fewer than sizeof(int) bytes remaining: _getw reports EOF. Binary mode streams.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#include <string.h>
static void mk(const char *n, const void *d, size_t len) { FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
#define FN "tc_chr_neg_getw_partial_int_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(two_bytes)
{
    FILE *f; mk(FN, "\x01\x02", 2); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), EOF); CHECK(feof(f) != 0); fclose(f); remove(FN);
}
TC_CASE(empty)
{
    FILE *f; mk(FN, "", 0); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), EOF); CHECK(feof(f) != 0); fclose(f); remove(FN);
}
TC_CASE(int_then_partial)
{
    FILE *f; int v = 0x04030201; unsigned char d[6]; memcpy(d, &v, 4); d[4] = 9; d[5] = 9;
    mk(FN, d, 6); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), 0x04030201); CHECK(feof(f) == 0);
    CHECK_EQ_I(_getw(f), EOF); CHECK(feof(f) != 0); fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(two_bytes); TC_RUN(empty); TC_RUN(int_then_partial); return tc_end(); }
