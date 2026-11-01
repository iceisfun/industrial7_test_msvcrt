/* TC
 * id:    dir_neg_findfirst_null
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _findfirst64i32 _findnext64i32 _wfindfirst64i32
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _findfirst, _findnext (NULL filespec/fileinfo: invalid parameter handler, errno EINVAL, return -1)
 * note:  _findfirst/_wfindfirst with a NULL filespec or NULL fileinfo: -1, EINVAL, one handler call (documented, passes).
 *        OBSERVED on Win10 22H2 ucrtbase: _findnext64i32 with a NULL fileinfo and a valid handle is not validated and the
 *        process dies with an access violation (0xC0000005, class C, asserted in a child process); _findnext64i32(-1, &fd)
 *        returns -1 with errno EINVAL.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

TC_DECL_PROC(intptr_t, _findfirst64i32, (const char *, struct _finddata64i32_t *));
TC_DECL_PROC(int, _findnext64i32, (intptr_t, struct _finddata64i32_t *));
TC_DECL_PROC(intptr_t, _wfindfirst64i32, (const wchar_t *, struct _wfinddata64i32_t *));
#define _findfirst64i32 tc_p__findfirst64i32
#define _findnext64i32 tc_p__findnext64i32
#define _wfindfirst64i32 tc_p__wfindfirst64i32

static const char *volatile v_star = "*";
static const wchar_t *volatile v_wstar = L"*";

TC_CASE(findfirst_null_spec)
{
    struct _finddata64i32_t fd; const char *volatile s = NULL; intptr_t h;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    h = _findfirst64i32(s, &fd);
    CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(findfirst_null_info)
{
    struct _finddata64i32_t *volatile p = NULL; intptr_t h;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    h = _findfirst64i32(v_star, p);
    CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wfindfirst_null)
{
    struct _wfinddata64i32_t fd; struct _wfinddata64i32_t *volatile p = NULL;
    const wchar_t *volatile s = NULL; intptr_t h;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    h = _wfindfirst64i32(s, &fd);
    CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    h = _wfindfirst64i32(v_wstar, p);
    CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
/* Class C: on Win10 22H2 ucrtbase _findnext with a NULL fileinfo or an invalid handle is not validated and faults (0xC0000005). Child processes. */
TC_CASE(findnext_null_info_crashes)
{
    DWORD code = 0; int ran = tc_spawn_self("fnnull", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0xC0000005u);
}
TC_CASE(findnext_bad_handle)
{
    struct _finddata64i32_t fd; volatile intptr_t h = -1; int r;
    tc_iph_install(); errno = 0;
    r = _findnext64i32(h, &fd);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);   /* observed */
}
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_findfirst64i32); TC_LOAD_PROC(_findnext64i32); TC_LOAD_PROC(_wfindfirst64i32);
    if (!tc_p__findfirst64i32 || !tc_p__findnext64i32 || !tc_p__wfindfirst64i32) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    if ((c = tc_child_name()) != NULL) {
        struct _finddata64i32_t fd; intptr_t h;
        tc_iph_install();
        struct _finddata64i32_t *volatile p = NULL;
        h = _findfirst64i32(v_star, &fd);
        (void)_findnext64i32(h, p);
        return 77;
    }
    TC_RUN(findfirst_null_spec); TC_RUN(findfirst_null_info); TC_RUN(wfindfirst_null);
    TC_RUN(findnext_null_info_crashes); TC_RUN(findnext_bad_handle);
    return tc_end();
}
