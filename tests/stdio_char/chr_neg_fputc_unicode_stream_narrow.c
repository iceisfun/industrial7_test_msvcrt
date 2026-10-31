/* TC
 * id:    chr_neg_fputc_unicode_stream_narrow
 * area:  stdio_char
 * funcs: fputc
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fopen ccs=; _write (odd byte count invalid in Unicode mode)
 * note:  PROBE: narrow fputc of 1 byte to a UTF-16LE stream. Output is buffered so failure may surface at fputc or at fflush; asserted: it fails at one of them with EINVAL and the handler fires.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#define FN "tc_chr_neg_fputc_unicode_stream_narrow_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(odd_byte)
{
    FILE *f; int r1, r2; volatile int c = 'a';
    if (!tc_iph_install()) TC_SKIP("no handler api");
    f = fopen(FN, "w,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r1 = fputc(c, f); r2 = fflush(f);
    CHECK(r1 == EOF || r2 == EOF);
    CHECK_EQ_I(errno, EINVAL);
    clearerr(f); fclose(f); remove(FN);
}
TC_CASE(handler_fires)
{
    FILE *f; volatile int c = 'a';
    if (!tc_iph_install()) TC_SKIP("no handler api");
    f = fopen(FN, "w,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; (void)fputc(c, f); (void)fflush(f);
    CHECK(tc_iph_count >= 1);
    clearerr(f); fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(odd_byte); TC_RUN(handler_fires); return tc_end(); }
