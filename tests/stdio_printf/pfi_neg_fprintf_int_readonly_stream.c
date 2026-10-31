/* TC
 * id:    pfi_neg_fprintf_int_readonly_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.6.1p3 (negative value if an output or encoding error occurred), 7.21.1 (error indicator); MS docs fprintf
 * note:  Writing to a stream opened "rb" is not defined by ISO C; real CRTs fail the write (negative return, error indicator set, no data written). Assertion is the observed behaviour.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *name = "tc_pfi_neg_fprintf_int_readonly_stream.tmp";

static void mkfile(void) { FILE *f = fopen(name, "wb"); if (f) { fputs("abc", f); fclose(f); } }
static long filesize(void)
{
    WIN32_FILE_ATTRIBUTE_DATA d;
    if (!GetFileAttributesExA(name, GetFileExInfoStandard, &d)) return -1;
    return (long)d.nFileSizeLow;
}

TC_CASE(readonly_write_fails)
{
    FILE *f; int n;
    mkfile(); f = fopen(name, "rb"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    n = fprintf(f, "%d", 12345);
    CHECK(n < 0);
    CHECK(ferror(f) != 0);
    fclose(f);
    CHECK_EQ_I(filesize(), 3);
    remove(name);
}
TC_CASE(then_writable_ok)
{
    FILE *r, *w; int n; const char *name2 = "tc_pfi_neg_fprintf_int_readonly_stream2.tmp";
    mkfile(); r = fopen(name, "rb"); w = fopen(name2, "wb");
    CHECK(r != NULL && w != NULL);
    if (r && w) {
        n = fprintf(r, "%d", 1); CHECK(n < 0);
        n = fprintf(w, "%d|%u", -12, 345u); CHECK_EQ_I(n, 7);
    }
    if (r) fclose(r);
    if (w) { fclose(w); }
    remove(name); remove(name2);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(readonly_write_fails); TC_RUN(then_writable_ok);
    return tc_end();
}
