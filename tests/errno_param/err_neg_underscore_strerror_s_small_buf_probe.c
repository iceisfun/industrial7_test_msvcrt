/* TC
 * id:    err_neg_underscore_strerror_s_small_buf_probe
 * area:  errno_param
 * kind:  negative
 * funcs: _strerror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _strerror_s
 * note:  Observed on Win10 22H2 19045 ucrtbase: _strerror_s(buf, 4, "abcdef") (message does not fit) terminates the process
 *        with STATUS_STACK_BUFFER_OVERRUN 0xC0000409 even though a returning invalid-parameter handler is installed (the CRT
 *        checks its internal strcpy_s with _ERRCHECK = fast-fail), so the old in-process expectation (nonzero return, NUL-
 *        terminated) cannot be observed. Moved to a child process (class C).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(small_buffer_fastfails_despite_handler)
{
    DWORD code = 0; int ran = tc_spawn_self("small", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);                     /* did not return */
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);  /* observed: internal _ERRCHECK fast-fails; the installed handler does not save it */
}
int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        char buf[8]; memset(buf, 'x', sizeof buf);
        if (!tc_iph_install()) return 78;   /* a returning handler is installed: the process still dies */
        errno = ENOENT;
        _strerror_s(buf, 4, "abcdef");
        return 77;
    }
    TC_RUN(small_buffer_fastfails_despite_handler);
    return tc_end();
}
