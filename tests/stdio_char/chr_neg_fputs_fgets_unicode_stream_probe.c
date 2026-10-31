/* TC
 * id:    chr_neg_fputs_fgets_unicode_stream_probe
 * area:  stdio_char
 * funcs: fputs fgets fopen fclose fflush fwrite
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fopen ccs=, fputs, fgets
 * note:  PROBE: narrow fputs/fgets on a stream opened with ccs=UTF-16LE. The CRT's "stream must be ANSI" validation is
 *        expected to fail the call: fputs returns EOF with errno EINVAL; fgets returns NULL with errno EINVAL.
 *        The asserted contract is lenient: either the call fails (with EINVAL) or it succeeds treating the bytes raw.
 *        Invalid-parameter handler is installed so the validation report is harmless.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_fputs_fgets_unicode_stream_probe_" TCS(TC_CRT_VER) ".tmp"

TC_CASE(fputs_odd_length)
{
    FILE *f; int r;
    f = fopen(FN, "w,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    r = fputs("abc", f);
    if (r < 0) CHECK_EQ_I(errno, EINVAL);
    else { errno = 0; r = fflush(f); if (r != 0) CHECK(errno != 0); }
    fclose(f); remove(FN);
}
TC_CASE(fputs_even_length)
{
    FILE *f; int r;
    f = fopen(FN, "w,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    r = fputs("ab", f);
    if (r == EOF) CHECK_EQ_I(errno, EINVAL);
    fclose(f); remove(FN);
}
TC_CASE(fgets_on_utf16)
{
    FILE *f; char buf[16]; char *p;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fwrite("\xff\xfe" "a\0b\0\n\0", 1, 8, f); fclose(f);
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    errno = 0;
    p = fgets(buf, (int)sizeof buf, f);
    if (!p) CHECK_EQ_I(errno, EINVAL);
    else CHECK_EQ_U((unsigned char)buf[0], 'a');
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(fputs_odd_length); TC_RUN(fputs_even_length); TC_RUN(fgets_on_utf16);
    return tc_end();
}
