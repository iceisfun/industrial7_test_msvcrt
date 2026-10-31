/* TC
 * id:    gdr_neg_findnext_32i64_bad_handle_probe
 * area:  dir
 * kind:  negative
 * funcs: _wfindfirst32i64 _wfindnext32i64 _findclose
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _findnext (returns -1 on failure)
 * note:  PROBE: same as gdr_neg_findnext_bad_handle_probe but for _wfindnext32i64 alone: handles -1, 0, closed give -1 with errno
 *        in {EINVAL,EBADF,ENOENT}; NULL fileinfo with valid handle gives -1/EINVAL and the handler runs.
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
    h = _wfindfirst32i64(v_wspec, &c); CHECK(h != -1);
    if (h != -1) { CHECK_EQ_I(_findclose(h), 0); BAD(h); }
}
TC_CASE(null_fileinfo)
{
    struct _wfinddata32i64_t c; struct _wfinddata32i64_t *volatile nc = NULL; intptr_t h;
    tc_iph_install();
    h = _wfindfirst32i64(v_wspec, &c); CHECK(h != -1);
    if (h != -1) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wfindnext32i64(h, nc), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); _findclose(h); }
}
int main(void) { tc_begin(); TC_RUN(bad_handles); TC_RUN(null_fileinfo); return tc_end(); }
