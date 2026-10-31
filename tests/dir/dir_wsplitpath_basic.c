/* TC
 * id:    dir_wsplitpath_basic
 * area:  dir
 * funcs: _wsplitpath
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _splitpath, _wsplitpath
 */
#include "tc.h"
#include <stdlib.h>

static const wchar_t *vw(const wchar_t *s) { static volatile const wchar_t *v; v = s; return (const wchar_t *)v; }

TC_CASE(full)
{
    wchar_t dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    _wsplitpath(vw(L"C:\\dir\\file.ext"), dr, di, fn, ex);
    CHECK_WSTR(dr, L"C:"); CHECK_WSTR(di, L"\\dir\\"); CHECK_WSTR(fn, L"file"); CHECK_WSTR(ex, L".ext");
}
TC_CASE(non_ascii)
{
    wchar_t dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    _wsplitpath(vw(L"D:\\d\x00e9\\caf\x00e9\x4e2d.t\x00fc"), dr, di, fn, ex);
    CHECK_WSTR(dr, L"D:"); CHECK_WSTR(di, L"\\d\x00e9\\"); CHECK_WSTR(fn, L"caf\x00e9\x4e2d"); CHECK_WSTR(ex, L".t\x00fc");
}
TC_CASE(forward)
{
    wchar_t dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    _wsplitpath(vw(L"/usr/local/x.c"), dr, di, fn, ex);
    CHECK_WSTR(dr, L""); CHECK_WSTR(di, L"/usr/local/"); CHECK_WSTR(fn, L"x"); CHECK_WSTR(ex, L".c");
}
TC_CASE(null_outputs)
{
    wchar_t a[_MAX_DRIVE], b[_MAX_DIR], c[_MAX_FNAME], d[_MAX_EXT];
    const wchar_t *p = vw(L"C:\\d\\f.e");
    _wsplitpath(p, NULL, b, c, d); CHECK_WSTR(b, L"\\d\\"); CHECK_WSTR(c, L"f"); CHECK_WSTR(d, L".e");
    _wsplitpath(p, a, NULL, c, d); CHECK_WSTR(a, L"C:"); CHECK_WSTR(c, L"f"); CHECK_WSTR(d, L".e");
    _wsplitpath(p, a, b, NULL, d); CHECK_WSTR(a, L"C:"); CHECK_WSTR(b, L"\\d\\"); CHECK_WSTR(d, L".e");
    _wsplitpath(p, a, b, c, NULL); CHECK_WSTR(a, L"C:"); CHECK_WSTR(b, L"\\d\\"); CHECK_WSTR(c, L"f");
}
int main(void) { tc_begin(); TC_RUN(full); TC_RUN(non_ascii); TC_RUN(forward); TC_RUN(null_outputs); return tc_end(); }
