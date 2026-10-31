/* TC
 * id:    err_errno_success_unchanged_probe
 * area:  errno_param
 * funcs: fopen fclose strtol _errno
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.5p3 (permits success calls to set errno nonzero; does not require unchanged)
 * note:  PROBE: successful strtol/fopen/fclose leave a pre-set nonzero errno exactly unchanged on Windows CRTs.
 */
#include "tc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

TC_CASE(strtol_ok) { volatile const char *s = "12"; long v; errno = EINVAL; v = strtol((const char *)s, NULL, 10); CHECK_EQ_I(v, 12); CHECK_EQ_I(errno, EINVAL); errno = 0; }
TC_CASE(fopen_fclose_ok)
{
    static const char name[] = "tc_err_errno_success_unchanged_probe.tmp";
    HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    FILE *f; int r;
    CHECK(h != INVALID_HANDLE_VALUE); if (h == INVALID_HANDLE_VALUE) return;
    CloseHandle(h);
    errno = ENOENT;
    f = fopen(name, "rb"); CHECK(f != NULL);
    CHECK_EQ_I(errno, ENOENT);
    if (f) { r = fclose(f); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, ENOENT); }
    errno = 0; DeleteFileA(name);
}
int main(void) { tc_begin(); TC_RUN(strtol_ok); TC_RUN(fopen_fclose_ok); return tc_end(); }
