/* TC
 * id:    dir_neg_wide_null_args
 * area:  dir
 * funcs: _wfullpath _wgetcwd _wsplitpath _wmakepath _wfindfirst64i32
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _getcwd (maxlen <= 0 invokes the invalid-parameter handler, EINVAL), _findfirst (EINVAL), _fullpath (NULL if longer than maxlen)
 * note:  NULL _wsplitpath/_wmakepath behaviour is not asserted: each runs in a child process whose outcome is only
 *        recorded (documented value for the NULL path is not stated for the non-_s functions).
 */
#include "tc.h"
#include <direct.h>
#include <io.h>
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>

static const wchar_t *volatile v_wnull;
static const wchar_t *volatile v_x = L"x";

TC_CASE(wgetcwd_zero_size)
{
    wchar_t wb[MAX_PATH];
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK(_wgetcwd(wb, 0) == NULL);
    CHECK_EQ_I(errno, EINVAL);
}

TC_CASE(wfindfirst_null)
{
    struct _wfinddata64i32_t fd;
    intptr_t h;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    h = _wfindfirst64i32(v_wnull, &fd);
    CHECK_EQ_I(h, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

TC_CASE(wfullpath_too_small)
{
    wchar_t wb[4] = { L'Q', 0, 0, 0 };
    tc_iph_install(); errno = 0;
    CHECK(_wfullpath(wb, v_x, 2) == NULL);   /* errno ERANGE is probed in dir_neg_probe_fullpath_erange */
}

TC_CASE(null_children_recorded)
{
    DWORD code = 0; int ran;
    ran = tc_spawn_self("splitpath", &code, 20000); CHECK(ran);
    tc_write(code == 77 ? "TC NOTE _wsplitpath(NULL) returned\n" : "TC NOTE _wsplitpath(NULL) did not return normally\n");
    ran = tc_spawn_self("makepath", &code, 20000); CHECK(ran);
    tc_write(code == 77 ? "TC NOTE _wmakepath(NULL) returned\n" : "TC NOTE _wmakepath(NULL) did not return normally\n");
}

static int child_main(const char *c)
{
    wchar_t d[MAX_PATH], dir[MAX_PATH], f[MAX_PATH], e[MAX_PATH];
    tc_iph_install();
    if (c[0] == 's') _wsplitpath(v_wnull, d, dir, f, e);
    else _wmakepath((wchar_t *)v_wnull, L"C:", L"\\d\\", L"f", L".e");
    return 77;
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(wgetcwd_zero_size); TC_RUN(wfindfirst_null); TC_RUN(wfullpath_too_small); TC_RUN(null_children_recorded);
    return tc_end();
}
