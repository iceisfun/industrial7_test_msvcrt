/* TC
 * id:    pfw_neg_fwprintf_readonly_ferror_state
 * area:  stdio_printf
 * funcs: fwprintf ferror clearerr fopen fgetc fclose remove
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.21.6.1 (fwprintf returns negative on error); EBADF for a write to a stream not opened for writing is CRT-internal behaviour (_flsbuf), not stated on the fwprintf page
 * note:  The invalid-parameter handler count is not asserted: EBADF on a read-only stream is a plain error return.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
#define FN "tc_pfw_neg_fwprintf_readonly_ferror_state_" TC_STR(TC_CRT_VER) ".tmp"

static const wchar_t *volatile fx = L"x";

static void mk(void) { FILE *f = fopen(FN, "wb"); if (f) { fputc('Q', f); fputc('R', f); fputc('S', f); fclose(f); } }
static long fsz(void)
{
    WIN32_FILE_ATTRIBUTE_DATA d;
    if (!GetFileAttributesExA(FN, GetFileExInfoStandard, &d)) return -1;
    return (long)d.nFileSizeLow;
}

TC_CASE(write_fails_ebadf)
{
    FILE *fp; int n; mk(); fp = fopen(FN, "r"); CHECK(fp != NULL); if (!fp) { remove(FN); return; }
    errno = 0; n = fwprintf(fp, fx);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EBADF);
    fclose(fp);
    CHECK_EQ_I(fsz(), 3);
    remove(FN);
}
TC_CASE(stream_still_readable)
{
    FILE *fp; mk(); fp = fopen(FN, "r"); CHECK(fp != NULL); if (!fp) { remove(FN); return; }
    CHECK_EQ_I(fwprintf(fp, fx), -1);
    clearerr(fp);
    CHECK_EQ_I(ferror(fp), 0);
    CHECK_EQ_I(fgetc(fp), 'Q');
    fclose(fp);
    remove(FN);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(write_fails_ebadf); TC_RUN(stream_still_readable);
    return tc_end();
}
