/* TC
 * id:    chr_neg_wrong_mode_stream
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fputc fgets fputs
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fgetc/fputc ("EOF ... error"; use ferror), C11 7.21.7 (error indicator set on failure)
 * note:  Reading a write-only stream / writing a read-only stream must fail with EOF/NULL and set the error indicator.
 *        errno values are probed separately in chr_neg_wrong_mode_errno_probe.
 */
#include "tc.h"
#include <stdio.h>

#define FN "tc_chr_neg_wrong_mode_stream_" TC_CRT_DLL ".tmp"

TC_CASE(read_from_writeonly)
{
    char b[8]; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK(ferror(f) != 0);
    clearerr(f);
    CHECK(fgets(b, 8, f) == NULL);
    CHECK(ferror(f) != 0);
    fclose(f); remove(FN);
}
TC_CASE(write_to_readonly)
{
    FILE *f = fopen(FN, "wb"); if (f) { fputs("abc", f); fclose(f); }
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fputc('x', f), EOF);
    CHECK(ferror(f) != 0);
    clearerr(f);
    CHECK_EQ_I(fputs("x", f), EOF);
    CHECK(ferror(f) != 0);
    clearerr(f);
    CHECK_EQ_I(ferror(f), 0);
    CHECK_EQ_I(fgetc(f), 'a');     /* file content untouched and still readable */
    fclose(f);
    f = fopen(FN, "rb"); if (f) { CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), 'c'); CHECK_EQ_I(fgetc(f), EOF); fclose(f); }
    remove(FN);
}
int main(void) { tc_begin(); TC_RUN(read_from_writeonly); TC_RUN(write_to_readonly); return tc_end(); }
