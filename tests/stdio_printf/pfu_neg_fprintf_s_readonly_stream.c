/* TC
 * id:    pfu_neg_fprintf_s_readonly_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fprintf_s (negative value on error); errno constants (EBADF)
 * note:  Probing exactly: fprintf_s on a stream opened "r" returns -1 (standard only says negative) and sets errno EBADF.
 *        Separate case asserts only what is certain (negative return, file content unchanged). ferror not asserted.
 *        Temp file tc_pfu_neg_fprintf_s_ro_*.tmp, removed.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define FN "tc_pfu_neg_fprintf_s_ro_" TC_CRT_DLL ".tmp"

TC_CASE(returns_negative_and_file_unchanged)
{
    FILE *f = fopen(FN, "rb"); int r; char buf[8] = {0}; size_t n;
    CHECK(f != NULL); if (!f) return;
    r = fprintf_s(f, "hello %d", 5);
    CHECK(r < 0);
    fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    n = fread(buf, 1, sizeof buf, f); fclose(f);
    CHECK_EQ_U(n, 1); CHECK_EQ_I(buf[0], 'x');
}
TC_CASE(probe_minus_one)
{
    FILE *f = fopen(FN, "rb"); int r;
    CHECK(f != NULL); if (!f) return;
    r = fprintf_s(f, "hello %d", 5); CHECK_EQ_I(r, -1);
    fclose(f);
}
TC_CASE(probe_errno_ebadf)
{
    FILE *f = fopen(FN, "rb");
    CHECK(f != NULL); if (!f) return;
    errno = 0; fprintf_s(f, "hello %d", 5); CHECK_EQ_I(errno, EBADF);
    fclose(f);
}
int main(void)
{
    FILE *f;
    tc_begin(); tc_iph_install();
    f = fopen(FN, "wb"); if (!f) return 2; fputc('x', f); fclose(f);
    TC_RUN(returns_negative_and_file_unchanged); TC_RUN(probe_minus_one); TC_RUN(probe_errno_ebadf);
    DeleteFileA(FN);
    return tc_end();
}
