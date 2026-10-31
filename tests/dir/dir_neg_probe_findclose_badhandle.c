/* TC
 * id:    dir_neg_probe_findclose_badhandle
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _findfirst64i32
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _findclose ("returns 0 if successful, -1 otherwise")
 * note:  Probes the errno left by _findclose on an invalid handle (-1) and on an already-closed handle: expected EBADF
 *        (ERROR_INVALID_HANDLE through the CRT's OS-error mapping). Only the -1 return is documented. The invalid-parameter
 *        handler count is not asserted.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

TC_DECL_PROC(intptr_t, _findfirst64i32, (const char *, struct _finddata64i32_t *));

TC_CASE(close_minus_one)
{
    volatile intptr_t h = -1; int r;
    tc_iph_install(); errno = 0;
    r = _findclose(h);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
}
TC_CASE(double_close)
{
    struct _finddata64i32_t fd; intptr_t h; int r;
    tc_iph_install();
    TC_LOAD_PROC(_findfirst64i32);
    if (!tc_p__findfirst64i32) TC_SKIP("no export");
    h = tc_p__findfirst64i32("*", &fd);
    CHECK(h != -1);
    if (h == -1) return;
    CHECK_EQ_I(_findclose(h), 0);
    errno = 0;
    r = _findclose(h);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
}
int main(void) { tc_begin(); TC_RUN(close_minus_one); TC_RUN(double_close); return tc_end(); }
