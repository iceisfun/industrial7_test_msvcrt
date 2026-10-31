/* TC
 * id:    err_const_core_values
 * area:  errno_param
 * funcs: _errno strtol fopen
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs errno constants (C11 7.5 only requires EDOM/ERANGE/EILSEQ to exist, not their values)
 * note:  Values are Microsoft-documented, not ISO. Header constants plus a runtime case tying CRT-set errno to the literals.
 */
#include "tc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

TC_CASE(sequence)
{
    static const int v[] = { EPERM, ENOENT, ESRCH, EINTR, EIO, ENXIO, E2BIG, ENOEXEC, EBADF, ECHILD, EAGAIN, ENOMEM, EACCES, EFAULT,
        EBUSY, EEXIST, EXDEV, ENODEV, ENOTDIR, EISDIR, EINVAL, ENFILE, EMFILE, ENOTTY, EFBIG, ENOSPC, ESPIPE, EROFS, EMLINK, EPIPE, EDOM, ERANGE };
    static const int w[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14, 16,17,18,19,20,21,22,23,24,25, 27,28,29,30,31,32,33,34 };
    size_t i; CHECK_EQ_U(sizeof v / sizeof v[0], sizeof w / sizeof w[0]);
    for (i = 0; i < sizeof v / sizeof v[0]; i++) CHECK_EQ_I(v[i], w[i]);
}
TC_CASE(spelled) { CHECK_EQ_I(EAGAIN, 11); CHECK_EQ_I(EACCES, 13); CHECK_EQ_I(EINVAL, 22); CHECK_EQ_I(EDOM, 33); CHECK_EQ_I(ERANGE, 34); CHECK_EQ_I(ENOENT, 2); CHECK_EQ_I(EBADF, 9); CHECK_EQ_I(ENOMEM, 12); }
TC_CASE(others) { CHECK_EQ_I(ENOTDIR, 20); CHECK_EQ_I(EISDIR, 21); CHECK_EQ_I(EMFILE, 24); CHECK_EQ_I(ENOSPC, 28); CHECK_EQ_I(EPIPE, 32); }
TC_CASE(runtime_literals)
{
    volatile const char *big = "99999999999999999999"; volatile const char *nm = "tc_err_const_core_missing_file.tmp";
    FILE *fp;
    errno = 0; (void)strtol((const char *)big, NULL, 10);
    CHECK_EQ_I(errno, 34);   /* ERANGE per MS docs strtol */
    errno = 0; fp = fopen((const char *)nm, "rb");
    if (fp) fclose(fp);
    CHECK(fp == NULL);
    CHECK_EQ_I(errno, 2);    /* ENOENT per MS docs fopen/_open */
    errno = 0;
}

int main(void) { tc_begin(); (void)_errno(); TC_RUN(sequence); TC_RUN(spelled); TC_RUN(others); TC_RUN(runtime_literals); return tc_end(); }
