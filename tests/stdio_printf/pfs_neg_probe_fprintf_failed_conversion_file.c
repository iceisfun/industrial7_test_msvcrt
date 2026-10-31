/* TC
 * id:    pfs_neg_probe_fprintf_failed_conversion_file
 * area:  stdio_printf
 * funcs: fprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf %ls / wcrtomb (EILSEQ)
 * note:  PROBE: narrow fprintf of "%ls" with U+20AC in the "C" locale fails with -1/EILSEQ; the failure is not a stream
 *        error (ferror==0); a later fprintf succeeds and its text ends the file. Prefix before the failure unasserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define TMP "tc_pfs_failconv.tmp"

TC_CASE(failed_conversion)
{
    FILE *f; int n; char buf[64]; size_t got;
    volatile wchar_t wc = 0x20ac; wchar_t ws[2];
    ws[0] = wc; ws[1] = 0;
    f = fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    n = fprintf(f, "%ls", ws);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(errno, EILSEQ);
    CHECK_EQ_I(ferror(f), 0);
    n = fprintf(f, "ok");
    CHECK_EQ_I(n, 2);
    CHECK_EQ_I(fclose(f), 0);
    f = fopen(TMP, "rb"); CHECK(f != NULL);
    if (f) {
        got = fread(buf, 1, sizeof buf - 1, f); buf[got] = 0; fclose(f);
        CHECK(got >= 2);
        if (got >= 2) CHECK_STR(buf + got - 2, "ok");
    }
    remove(TMP);
}
int main(void) { tc_begin(); TC_RUN(failed_conversion); return tc_end(); }
