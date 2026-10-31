/* TC
 * id:    dir_wchdir_wgetcwd_roundtrip
 * area:  dir
 * funcs: _wchdir _wgetcwd _wmkdir _wrmdir
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _chdir/_wchdir, _getcwd/_wgetcwd
 */
#include "tc.h"
#include <direct.h>
#include <wchar.h>

static const wchar_t * volatile sub = L"tc_dir_wchdir_wgetcwd_roundtrip_\u00e9\u4e2d";

TC_CASE(wide_roundtrip)
{
    wchar_t orig[MAX_PATH], c[MAX_PATH], k[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, orig);
    CHECK_EQ_I(_wmkdir(sub), 0);
    CHECK_EQ_I(_wchdir(sub), 0);
    CHECK(_wgetcwd(c, MAX_PATH) != NULL);
    GetCurrentDirectoryW(MAX_PATH, k);
    CHECK_WSTR(c, k);
    {
        size_t n = 0, m = 0; while (c[n]) n++; while (sub[m]) m++;
        CHECK(n > m);
        if (n > m) { CHECK_WSTR(c + n - m, sub); CHECK(c[n - m - 1] == L'\\'); }
    }
    CHECK_EQ_I(_wchdir(L".."), 0);
    CHECK(_wgetcwd(c, MAX_PATH) != NULL);
    CHECK_WSTR(c, orig);
    SetCurrentDirectoryW(orig);
    CHECK_EQ_I(_wrmdir(sub), 0);
}
int main(void)
{
    tc_begin();
    RemoveDirectoryW(sub);
    TC_RUN(wide_roundtrip);
    RemoveDirectoryW(sub);
    return tc_end();
}
