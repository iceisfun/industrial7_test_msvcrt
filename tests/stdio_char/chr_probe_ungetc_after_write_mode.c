/* TC
 * id:    chr_probe_ungetc_after_write_mode
 * area:  stdio_char
 * funcs: ungetc fputc
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.21.5.3p7 (read after write needs fseek/rewind); MS docs ungetc
 * note:  PROBE: whether ungetc on a w+ stream directly after a write (no seek) is rejected with EOF (expected).
 *        OBSERVED (Win10 22H2, class A): ungetc('b') on a w+b stream right after fputc is accepted (returns 98) and fgetc then returns 'b' (raw: ungetc(b, f) == EOF got=98).
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
#define FN "tc_chr_probe_ungetc_after_write_mode_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(ungetc_rejected)
{
    FILE *f = fopen(FN, "w+b");
    volatile int a = 'a', b = 'b';
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    CHECK_EQ_I(fputc(a, f), 'a');
    CHECK_EQ_I(ungetc(b, f), 'b');   /* observed: accepted, not rejected */
    CHECK_EQ_I(fgetc(f), 'b');       /* and the pushed-back char is read back */
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(ungetc_rejected); return tc_end(); }
