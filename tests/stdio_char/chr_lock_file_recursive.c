/* TC
 * id:    chr_lock_file_recursive
 * area:  stdio_char
 * funcs: _lock_file _unlock_file fgetc fputc
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _lock_file / _unlock_file (per-stream locks are recursive critical sections)
 * note:  msvcrt.dll excluded (no _lock_file import). no threads/timing: a deadlock would show up as a harness timeout.
 */
#include "tc.h"
#include <stdio.h>

static void wr(const char *n, const void *d, size_t len)
{ FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
static size_t rd(const char *n, unsigned char *b, size_t max)
{ FILE *f = fopen(n, "rb"); size_t r = 0; if (f) { r = fread(b, 1, max, f); fclose(f); } return r; }
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define FN "tc_chr_lock_file_recursive_" TC_S(TC_CRT_VER) ".tmp"

TC_CASE(nested_put)
{
    unsigned char b[8]; size_t n; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    _lock_file(f); _lock_file(f);
    CHECK_EQ_I(fputc('a', f), 'a'); CHECK_EQ_I(fputc('b', f), 'b');
    _unlock_file(f); CHECK_EQ_I(fputc('c', f), 'c'); _unlock_file(f);
    CHECK_EQ_I(fputc('d', f), 'd');
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 4); CHECK_MEM(b, "abcd", 4); remove(FN);
}
TC_CASE(get_after_unlock)
{
    FILE *f; wr(FN, "wxyz", 4); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    _lock_file(f); _lock_file(f);
    CHECK_EQ_I(fgetc(f), 'w');
    _unlock_file(f); _unlock_file(f);
    CHECK_EQ_I(fgetc(f), 'x'); CHECK_EQ_I(fgetc(f), 'y'); CHECK_EQ_I(fgetc(f), 'z'); CHECK_EQ_I(fgetc(f), EOF);
    fclose(f); remove(FN);
}
TC_CASE(triple)
{
    FILE *f; wr(FN, "12", 2); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    _lock_file(f); _lock_file(f); _lock_file(f);
    CHECK_EQ_I(fgetc(f), '1'); CHECK_EQ_I(fgetc(f), '2'); CHECK_EQ_I(fgetc(f), EOF);
    _unlock_file(f); _unlock_file(f); _unlock_file(f);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(nested_put); TC_RUN(get_after_unlock); TC_RUN(triple); return tc_end(); }
