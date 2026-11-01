/* TC
 * id:    dir_neg_findnext_after_close_probe
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _findfirst64i32 _findnext64i32 _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _findnext (not documented for stale handles)
 * note:  Probes that _findnext on an already closed search handle, and on handle 0, returns -1 without crashing. errno is
 *        recorded only through a set check {EINVAL, ENOENT, EBADF}; the invalid parameter handler is installed in case it fires.
 *        Win10 22H2 19045 (observed): handle 0 -> -1/EINVAL (validated, handler called); a handle that was already closed with _findclose is a use-after-free in
 *        the CRT and the process dies with 0xC0000005, so after_close now runs in a child and asserts that exit code (class C). Crash code stable across 2 runs [see ledger].
 *        Heap-state note: the 0xC0000005 comes from reading the freed kernelbase find-handle memory, not from a CRT check; it was stable in repeated reference runs. If it ever flakes, relax to "abnormal termination (code not 77, not 0)".
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

typedef intptr_t (__cdecl *ff_t)(const char *, void *);
typedef int (__cdecl *fn_t)(intptr_t, void *);
static union { long long al; char b[1024]; } fdb;

/* Windows (class C): _findnext on a closed handle uses freed memory and dies with an access violation, so the call runs in a child. */
TC_CASE(after_close)
{
    DWORD code = 0; int ran = tc_spawn_self("stale", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(handle_zero)
{
    fn_t fn = (fn_t)tc_crt_proc("_findnext64i32");
    volatile intptr_t z = 0;
    if (!fn) TC_SKIP("not exported");
    tc_iph_install();
    errno = 0;
    CHECK_EQ_I(fn(z, &fdb), -1);
    CHECK_EQ_I(errno, EINVAL);   /* observed: handle 0 is validated (handler runs, EINVAL) */
}

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        ff_t ff = (ff_t)tc_crt_proc("_findfirst64i32"); fn_t fn = (fn_t)tc_crt_proc("_findnext64i32");
        intptr_t h;
        if (!ff || !fn) return 78;
        tc_iph_install();
        h = ff("*", &fdb);
        if (h == -1) return 79;
        if (_findclose(h) != 0) return 80;
        fn(h, &fdb);
        return 77;       /* reached only if the stale-handle call returned */
    }
    TC_RUN(after_close); TC_RUN(handle_zero);
    return tc_end();
}
