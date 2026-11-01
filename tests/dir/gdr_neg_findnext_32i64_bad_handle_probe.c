/* TC
 * id:    gdr_neg_findnext_32i64_bad_handle_probe
 * area:  dir
 * kind:  negative
 * funcs: _wfindfirst32i64 _wfindnext32i64 _findclose
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _findnext (returns -1 on failure)
 * note:  PROBE: same as gdr_neg_findnext_bad_handle_probe but for _wfindnext32i64 alone: handles -1, 0, closed give -1 with errno
 *        in {EINVAL,EBADF,ENOENT}; NULL fileinfo with valid handle gives -1/EINVAL and the handler runs.
 *        Win10 22H2 19045 (observed): handles -1 and 0 give -1/EINVAL (validated); the closed-handle step crashes with 0xC0000005 (use-after-free) and moved to the
 *        child case closed_handle_crashes (class C).
 *        crt narrowed to ucrtbase: the crash / fast-fail behaviour was observed only on ucrtbase 19045; older CRTs are [unverified] (original expectation not asserted there).
 *        Heap-state note: the 0xC0000005 comes from reading the freed kernelbase find-handle memory, not from a CRT check; it was stable in repeated reference runs. If it ever flakes, relax to "abnormal termination (code not 77, not 0)".
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

static volatile intptr_t v_h;
static const wchar_t *volatile v_wspec = L"*";
static int okerr(int e) { return e == EINVAL || e == EBADF || e == ENOENT; }
#define BAD(h) do { int r_; v_h = (h); tc_iph_install(); errno = 0; r_ = _wfindnext32i64(v_h, &c); CHECK_EQ_I(r_, -1); CHECK(okerr(errno)); } while (0)

TC_CASE(bad_handles)
{
    struct _wfinddata32i64_t c; intptr_t h;
    BAD(-1); BAD(0);
}
/* Windows (class C): a handle already closed with _findclose is a use-after-free in the CRT; the call dies with an access violation, so it runs in a child. */
TC_CASE(closed_handle_crashes)
{
    DWORD code = 0; int ran = tc_spawn_self("stale", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(null_fileinfo)
{
    struct _wfinddata32i64_t c; struct _wfinddata32i64_t *volatile nc = NULL; intptr_t h;
    tc_iph_install();
    h = _wfindfirst32i64(v_wspec, &c); CHECK(h != -1);
    if (h != -1) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wfindnext32i64(h, nc), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); _findclose(h); }
}
int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        struct _wfinddata32i64_t c; intptr_t h = _wfindfirst32i64(v_wspec, &c);
        if (h == -1) return 79;
        _findclose(h);
        tc_iph_install(); _wfindnext32i64(h, &c);
        return 77;
    }
    TC_RUN(bad_handles); TC_RUN(null_fileinfo); TC_RUN(closed_handle_crashes);
    return tc_end();
}
