/* TC
 * id:    err_const_ext_values
 * area:  errno_param
 * funcs: _errno
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs errno constants; errno.h
 * note:  Header-level values only (ISO fixes none of these numbers).
 */
#include "tc.h"
#include <errno.h>

TC_CASE(values) { CHECK_EQ_I(EDEADLK, 36); CHECK_EQ_I(EDEADLOCK, EDEADLK); CHECK_EQ_I(ENAMETOOLONG, 38); CHECK_EQ_I(ENOLCK, 39); CHECK_EQ_I(ENOSYS, 40); CHECK_EQ_I(ENOTEMPTY, 41); CHECK_EQ_I(EILSEQ, 42); }
TC_CASE(strunc) {
#if TC_AT_LEAST(80)
    CHECK_EQ_I(STRUNCATE, 80);
#else
    TC_SKIP("STRUNCATE not in this CRT");
#endif
}
TC_CASE(distinct)
{
    static const int v[] = { EPERM, ENOENT, ESRCH, EINTR, EIO, ENXIO, E2BIG, ENOEXEC, EBADF, ECHILD, EAGAIN, ENOMEM, EACCES, EFAULT, EBUSY,
        EEXIST, EXDEV, ENODEV, ENOTDIR, EISDIR, EINVAL, ENFILE, EMFILE, ENOTTY, EFBIG, ENOSPC, ESPIPE, EROFS, EMLINK, EPIPE, EDOM, ERANGE,
        EDEADLK, ENAMETOOLONG, ENOLCK, ENOSYS, ENOTEMPTY, EILSEQ };
    size_t i, j, n = sizeof v / sizeof v[0];
    for (i = 0; i < n; i++) { CHECK(v[i] > 0); for (j = i + 1; j < n; j++) CHECK(v[i] != v[j]); }
}
int main(void) { tc_begin(); (void)_errno(); TC_RUN(values); TC_RUN(strunc); TC_RUN(distinct); return tc_end(); }
