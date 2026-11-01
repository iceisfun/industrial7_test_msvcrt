/* TC
 * id:    sec_strerror_s_basic
 * area:  secure_crt
 * funcs:
 * procs: strerror_s _strerror_s _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs strerror_s, _strerror_s; strerror table of messages
 * note:  [observed on ucrtbase 19045 only; the same expectation on the legacy CRTs listed in crt: is [unverified]] OBSERVED on Win10 22H2 (19045): _strerror_s(buf,n,"myprefix") with errno 22 yields "myprefix: Invalid argument" and
 *        with a NULL prefix "No such file or directory" (errno 2) -- NO trailing newline (unlike the non-_s _strerror
 *        documented behaviour). The strerror_s message table entries match the docs. Stable.
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *strerror_s_t)(char *, size_t, int);
typedef int (__cdecl *_strerror_s_t)(char *, size_t, const char *);
typedef int *(__cdecl *errno_fn)(void);
static strerror_s_t p_se; static _strerror_s_t p_use; static errno_fn p_errno;

TC_CASE(errno0_No_error) { char b[128]; CHECK_EQ_I(p_se(b, sizeof b, 0), 0); CHECK_STR(b, "No error"); }
TC_CASE(EINVAL_Invalid_argument) { char b[128]; CHECK_EQ_I(p_se(b, sizeof b, 22), 0); CHECK_STR(b, "Invalid argument"); }
TC_CASE(ENOENT_No_such_file_or_directory) { char b[128]; CHECK_EQ_I(p_se(b, sizeof b, 2), 0); CHECK_STR(b, "No such file or directory"); }
TC_CASE(ERANGE_Result_too_large) { char b[128]; CHECK_EQ_I(p_se(b, sizeof b, 34), 0); CHECK_STR(b, "Result too large"); }
TC_CASE(strerror_underscore_prefix_colon_space_msg)
{
    char b[128];
    if (!p_use || !p_errno) TC_SKIP("_strerror_s/_errno not exported");
    *p_errno() = 22;
    CHECK_EQ_I(p_use(b, sizeof b, "myprefix"), 0);
    CHECK_STR(b, "myprefix: Invalid argument");   /* observed: no trailing newline */
    *p_errno() = 0;
}
TC_CASE(strerror_underscore_null_prefix_msg)
{
    char b[128];
    if (!p_use || !p_errno) TC_SKIP("_strerror_s/_errno not exported");
    *p_errno() = 2;
    CHECK_EQ_I(p_use(b, sizeof b, NULL), 0);
    CHECK_STR(b, "No such file or directory");   /* observed: no trailing newline */
    *p_errno() = 0;
}
int main(void)
{
    tc_begin();
    p_se = (strerror_s_t)tc_crt_proc("strerror_s"); p_use = (_strerror_s_t)tc_crt_proc("_strerror_s"); p_errno = (errno_fn)tc_crt_proc("_errno");
    CHECK(p_se); if (!p_se) return tc_end();
    TC_RUN(errno0_No_error); TC_RUN(EINVAL_Invalid_argument); TC_RUN(ENOENT_No_such_file_or_directory); TC_RUN(ERANGE_Result_too_large);
    TC_RUN(strerror_underscore_prefix_colon_space_msg); TC_RUN(strerror_underscore_null_prefix_msg);
    return tc_end();
}
