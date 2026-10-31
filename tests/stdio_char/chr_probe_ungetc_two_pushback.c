/* TC
 * id:    chr_probe_ungetc_two_pushback
 * area:  stdio_char
 * funcs: ungetc fgetc
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.7.10 (only one pushback guaranteed)
 * note:  PROBE: after reading 'a','b' from a buffered binary stream, ungetc('b') then ungetc('a') both succeed on the MS CRT
 *        (pushback moves the buffer pointer back while buffer space exists); fgetc then yields a,b,c.
 */
#include "tc.h"
#include <stdio.h>

static void wr(const char *n, const void *d, size_t len)
{ FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
static size_t rd(const char *n, unsigned char *b, size_t max)
{ FILE *f = fopen(n, "rb"); size_t r = 0; if (f) { r = fread(b, 1, max, f); fclose(f); } return r; }
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define FN "tc_chr_probe_ungetc_two_pushback_" TC_S(TC_CRT_VER) ".tmp"
static volatile int vc;

TC_CASE(two)
{
    FILE *f; wr(FN, "abc", 3); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b');
    vc = 'b'; CHECK_EQ_I(ungetc(vc, f), 'b');
    vc = 'a'; CHECK_EQ_I(ungetc(vc, f), 'a');
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), 'c'); CHECK_EQ_I(fgetc(f), EOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(two); return tc_end(); }
