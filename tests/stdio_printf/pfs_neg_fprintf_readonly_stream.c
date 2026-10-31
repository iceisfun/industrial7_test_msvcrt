/* TC
 * id:    pfs_neg_fprintf_readonly_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fwprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1 (output to a stream not open for writing fails); MS CRT _flsbuf behaviour
 * note:  PROBE: expected -1, ferror(f) set and errno EBADF when writing to a stream opened "r". EBADF is classic CRT behaviour
 *        (_flsbuf) but not individually documented. The file contents must remain untouched.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>

#define FN "tc_pfs_neg_fprintf_readonly_stream.tmp"
static const char * volatile sfmt = "%s";
static const wchar_t * volatile wsfmt = L"%s";

static void mk(void) { FILE *f = fopen(FN, "w"); if (f) { fputs("orig", f); fclose(f); } }
static void check_untouched(void)
{
    char b[16] = {0}; FILE *f = fopen(FN, "r");
    CHECK(f != NULL); if (!f) return;
    fgets(b, sizeof b, f); fclose(f);
    CHECK_STR(b, "orig");
}
TC_CASE(fprintf_readonly)
{
    FILE *f; int r; mk();
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    errno = 0; r = fprintf(f, sfmt, "x");
    CHECK_EQ_I(r, -1);
    CHECK(ferror(f) != 0);
    CHECK_EQ_I(errno, EBADF);
    fclose(f); check_untouched(); remove(FN);
}
TC_CASE(fwprintf_readonly)
{
    FILE *f; int r; mk();
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    errno = 0; r = fwprintf(f, wsfmt, L"x");
    CHECK(r < 0);
    CHECK(ferror(f) != 0);
    CHECK_EQ_I(errno, EBADF);
    fclose(f); check_untouched(); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(fprintf_readonly); TC_RUN(fwprintf_readonly); return tc_end(); }
