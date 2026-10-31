/* TC
 * id:    chr_probe_ungetc_at_start
 * area:  stdio_char
 * funcs: ungetc fgetc
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.7.10 (unspecified at position 0 for binary); MS docs ungetc
 * note:  PROBE: C leaves ungetc at file position 0 of a binary stream unspecified; MS keeps a separate buffer so we expect
 *        success, with the pushed-back byte read before the file content.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *name, const char *data, size_t n)
{
    FILE *f = fopen(name, "wb");
    if (f) { if (n) fwrite(data, 1, n, f); fclose(f); }
}

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_chr_probe_ungetc_at_start_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(unget_then_read)
{
    FILE *f; volatile int c = 'Z';
    mk(FN, "ab", 2);
    f = fopen(FN, "rb");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    CHECK_EQ_I(ungetc(c, f), 'Z');
    CHECK_EQ_I(fgetc(f), 'Z');
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(fgetc(f), 'b');
    CHECK_EQ_I(fgetc(f), EOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(unget_then_read); return tc_end(); }
