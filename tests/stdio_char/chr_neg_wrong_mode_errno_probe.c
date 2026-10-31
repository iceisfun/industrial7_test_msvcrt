/* TC
 * id:    chr_neg_wrong_mode_errno_probe
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fputc
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   none (MS docs do not state errno for wrong-mode stream I/O)
 * note:  Probes: fgetc on a write-only stream and fputc on a read-only stream set errno == EBADF (the CRT maps the failing _read/_write
 *        to EBADF). Most likely value; not documented for these functions.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_wrong_mode_errno_probe_a" TCV_S(TC_CRT_VER) ".tmp"

TC_CASE(fgetc_writeonly)
{
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    errno = 0; CHECK_EQ_I(fgetc(f), EOF); CHECK_EQ_I(errno, EBADF);
    fclose(f); remove(FN);
}
TC_CASE(fputc_readonly)
{
    FILE *f = fopen(FN, "wb"); if (f) { fputs("abc", f); fclose(f); }
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    errno = 0; CHECK_EQ_I(fputc('x', f), EOF); CHECK_EQ_I(errno, EBADF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(fgetc_writeonly); TC_RUN(fputc_readonly); return tc_end(); }
