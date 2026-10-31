/* TC
 * id:    dir_neg_findnext_after_close_probe
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _findfirst64i32 _findnext64i32 _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _findnext (not documented for stale handles)
 * note:  Probes that _findnext on an already closed search handle, and on handle 0, returns -1 without crashing. errno is
 *        recorded only through a set check {EINVAL, ENOENT, EBADF}; the invalid parameter handler is installed in case it fires.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

typedef intptr_t (__cdecl *ff_t)(const char *, void *);
typedef int (__cdecl *fn_t)(intptr_t, void *);
static union { long long al; char b[1024]; } fdb;

TC_CASE(after_close)
{
    ff_t ff = (ff_t)tc_crt_proc("_findfirst64i32"); fn_t fn = (fn_t)tc_crt_proc("_findnext64i32");
    intptr_t h; int r;
    if (!ff || !fn) TC_SKIP("not exported");
    tc_iph_install();
    h = ff("*", &fdb);
    CHECK(h != -1);
    if (h == -1) return;
    CHECK_EQ_I(_findclose(h), 0);
    errno = 0; r = fn(h, &fdb);
    CHECK_EQ_I(r, -1);
    CHECK(errno == EINVAL || errno == ENOENT || errno == EBADF);
}
TC_CASE(handle_zero)
{
    fn_t fn = (fn_t)tc_crt_proc("_findnext64i32");
    volatile intptr_t z = 0;
    if (!fn) TC_SKIP("not exported");
    tc_iph_install();
    CHECK_EQ_I(fn(z, &fdb), -1);
}

int main(void)
{
    tc_begin();
    TC_RUN(after_close); TC_RUN(handle_zero);
    return tc_end();
}
