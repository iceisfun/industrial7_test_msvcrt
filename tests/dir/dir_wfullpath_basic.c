/* TC
 * id:    dir_wfullpath_basic
 * area:  dir
 * funcs: _wfullpath _wgetcwd
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _fullpath, _wfullpath; _getcwd, _wgetcwd
 */
#include "tc.h"
#include <stdlib.h>
#include <direct.h>

static void expect(wchar_t *out, const wchar_t *rel)
{
    size_t n; GetCurrentDirectoryW(MAX_PATH, out);
    for (n = 0; out[n]; n++) ;
    if (out[n - 1] != L'\\') out[n++] = L'\\';
    while ((out[n++] = *rel++) != 0) ;
}
static wchar_t *vw(const wchar_t *s) { static volatile const wchar_t *v; v = s; return (wchar_t *)v; }

TC_CASE(relative)
{
    wchar_t buf[MAX_PATH], want[MAX_PATH];
    expect(want, L"file.txt");
    CHECK(_wfullpath(buf, vw(L"file.txt"), MAX_PATH) == buf);
    CHECK_WSTR(buf, want);
}
TC_CASE(dotdot)
{
    wchar_t buf[MAX_PATH], want[MAX_PATH];
    expect(want, L"x");
    CHECK(_wfullpath(buf, vw(L"sub\\..\\x"), MAX_PATH) != NULL);
    CHECK_WSTR(buf, want);
}
TC_CASE(forward_slash)
{
    wchar_t buf[MAX_PATH], want[MAX_PATH];
    expect(want, L"a\\b\\c.txt");
    CHECK(_wfullpath(buf, vw(L"a/b/c.txt"), MAX_PATH) != NULL);
    CHECK_WSTR(buf, want);
}
TC_CASE(non_ascii)
{
    wchar_t buf[MAX_PATH], want[MAX_PATH];
    expect(want, L"\x00e9t\x00e9\x4e2d.txt");
    CHECK(_wfullpath(buf, vw(L"\x00e9t\x00e9\x4e2d.txt"), MAX_PATH) != NULL);
    CHECK_WSTR(buf, want);
}
TC_CASE(null_buffer_allocates)
{
    wchar_t want[MAX_PATH]; wchar_t *r;
    expect(want, L"alloc.txt");
    r = _wfullpath(NULL, vw(L"alloc.txt"), 0);
    CHECK(r != NULL);
    if (r) { CHECK_WSTR(r, want); free(r); }
}
TC_CASE(wgetcwd_matches)
{
    wchar_t a[MAX_PATH], b[MAX_PATH]; wchar_t *r;
    GetCurrentDirectoryW(MAX_PATH, a);
    r = _wgetcwd(b, MAX_PATH);
    CHECK(r == b);
    CHECK_WSTR(b, a);
}
int main(void)
{
    tc_begin(); TC_RUN(relative); TC_RUN(dotdot); TC_RUN(forward_slash); TC_RUN(non_ascii);
    TC_RUN(null_buffer_allocates); TC_RUN(wgetcwd_matches);
    return tc_end();
}
