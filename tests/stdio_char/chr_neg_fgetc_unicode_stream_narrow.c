/* TC
 * id:    chr_neg_fgetc_unicode_stream_narrow
 * area:  stdio_char
 * funcs: fgetc
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fopen ccs=
 * note:  PROBE: narrow fgetc on a UTF-16LE read stream. Asserted: result is EOF or the first raw byte 0x61; if EOF, errno EINVAL.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#define FN "tc_chr_neg_fgetc_unicode_stream_narrow_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(narrow_read)
{
    FILE *f; int r; FILE *w; /* the CRT may raise the invalid-parameter handler; default handler is fatal */
    tc_iph_install(); w = fopen(FN, "wb"); CHECK(w != NULL); if (!w) return;
    fwrite("\xff\xfe" "a\0b\0", 1, 6, w); fclose(w);
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    errno = 0; r = fgetc(f);
    CHECK(r == EOF || r == 0x61);
    if (r == EOF) CHECK_EQ_I(errno, EINVAL);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(narrow_read); return tc_end(); }
