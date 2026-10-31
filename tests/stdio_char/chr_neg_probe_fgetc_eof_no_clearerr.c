/* TC
 * id:    chr_neg_probe_fgetc_eof_no_clearerr
 * area:  stdio_char
 * funcs: fgetc feof fopen fclose fputc
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.7.1 (EOF indicator semantics); MS docs fgetc
 * note:  PROBE: C11 7.21.7.1 reads as if fgetc must keep returning EOF while the end-of-file indicator is set; the MS
 *        _filbuf is believed not to test _IOEOF, so a later fgetc retries the read (a deviation from C, hence a probe). After fgetc==EOF, another handle appends 'b'; fgetc WITHOUT clearerr is
 *        expected to return 'b'. If real Windows returns sticky EOF this probe shows it.
 */
#include "tc.h"
#include <stdio.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_probe_fgetc_eof_no_clearerr_" TCS(TC_CRT_VER) ".tmp"

TC_CASE(retry_after_eof)
{
    FILE *w, *r, *a; int c;
    w = fopen(FN, "wb"); CHECK(w != NULL); if (!w) return;
    fputc('a', w); fclose(w);
    r = fopen(FN, "rb"); CHECK(r != NULL); if (!r) { remove(FN); return; }
    CHECK_EQ_I((fgetc)(r), 'a');
    CHECK_EQ_I((fgetc)(r), EOF);
    CHECK(feof(r) != 0);
    a = fopen(FN, "ab"); CHECK(a != NULL);
    if (a) { fputc('b', a); fclose(a); }
    c = (fgetc)(r);
    CHECK_EQ_I(c, 'b');
    fclose(r); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(retry_after_eof); return tc_end(); }
