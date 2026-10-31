/* TC
 * id:    gdr_neg_findnext_bad_handle_probe
 * area:  dir
 * kind:  negative
 * funcs: _findfirst32 _findnext32 _wfindfirst32 _wfindnext32  _wfindnext64 _wfindfirst64 _findclose
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _findnext (returns -1 on failure)
 * note:  PROBE: invalid handles (-1, 0, a handle already passed to _findclose) make _findnext* return -1 with errno in
 *        {EINVAL,EBADF,ENOENT}; NULL fileinfo with a valid handle gives -1/EINVAL with the invalid-parameter handler invoked.
 *        (_wfindnext32i64 is in gdr_neg_findnext_32i64_bad_handle_probe.) A closed handle is only used immediately after _findclose, with no intervening allocations.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

static volatile intptr_t v_h;
static const char *volatile v_spec = "*";
static const wchar_t *volatile v_wspec = L"*";
static int okerr(int e) { return e == EINVAL || e == EBADF || e == ENOENT; }
#define BAD(call) do { int r_; tc_iph_install(); errno = 0; r_ = (call); CHECK_EQ_I(r_, -1); CHECK(okerr(errno)); } while (0)

static void all_bad(intptr_t h)
{
    struct _finddata32_t a; struct _wfinddata32_t b; struct _wfinddata64_t d;
    v_h = h;
    BAD(_findnext32(v_h, &a));
    BAD(_wfindnext32(v_h, &b));
    BAD(_wfindnext64(v_h, &d));
}
TC_CASE(handle_minus_one) { all_bad(-1); }
TC_CASE(handle_zero) { all_bad(0); }
TC_CASE(handle_closed)
{
    struct _finddata32_t a; struct _wfinddata32_t b; struct _wfinddata64_t d; intptr_t h;
    h = _findfirst32(v_spec, &a); CHECK(h != -1);
    if (h != -1) { CHECK_EQ_I(_findclose(h), 0); v_h = h; BAD(_findnext32(v_h, &a)); }
    h = _wfindfirst32(v_wspec, &b); CHECK(h != -1);
    if (h != -1) { CHECK_EQ_I(_findclose(h), 0); v_h = h; BAD(_wfindnext32(v_h, &b)); }
    h = _wfindfirst64(v_wspec, &d); CHECK(h != -1);
    if (h != -1) { CHECK_EQ_I(_findclose(h), 0); v_h = h; BAD(_wfindnext64(v_h, &d)); }
}
TC_CASE(null_fileinfo)
{
    struct _finddata32_t a; struct _wfinddata32_t b; struct _wfinddata64_t d; intptr_t h;
    struct _finddata32_t *volatile na = NULL; struct _wfinddata32_t *volatile nb = NULL;
    struct _wfinddata64_t *volatile nd = NULL;
    tc_iph_install();
    h = _findfirst32(v_spec, &a); CHECK(h != -1);
    if (h != -1) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_findnext32(h, na), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); _findclose(h); }
    h = _wfindfirst32(v_wspec, &b); CHECK(h != -1);
    if (h != -1) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wfindnext32(h, nb), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); _findclose(h); }
    h = _wfindfirst64(v_wspec, &d); CHECK(h != -1);
    if (h != -1) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wfindnext64(h, nd), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); _findclose(h); }
}
int main(void) { tc_begin(); TC_RUN(handle_minus_one); TC_RUN(handle_zero); TC_RUN(handle_closed); TC_RUN(null_fileinfo); return tc_end(); }
