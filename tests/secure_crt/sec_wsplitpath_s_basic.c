/* TC
 * id:    sec_wsplitpath_s_basic
 * area:  secure_crt
 * funcs: _wsplitpath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _splitpath_s, _wsplitpath_s

 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
static wchar_t dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
static int sp(const wchar_t *p) { return _wsplitpath_s(p, dr, _MAX_DRIVE, di, _MAX_DIR, fn, _MAX_FNAME, ex, _MAX_EXT); }
TC_CASE(full_path)
{
    CHECK_EQ_I(sp(L"D:\\x\\y\\name.ext"), 0);
    CHECK_WSTR(dr, L"D:"); CHECK_WSTR(di, L"\\x\\y\\"); CHECK_WSTR(fn, L"name"); CHECK_WSTR(ex, L".ext");
}
TC_CASE(no_drive)
{
    CHECK_EQ_I(sp(L"/x/n.e"), 0);
    CHECK_WSTR(dr, L""); CHECK_WSTR(di, L"/x/"); CHECK_WSTR(fn, L"n"); CHECK_WSTR(ex, L".e");
}
TC_CASE(filename_only)
{
    CHECK_EQ_I(sp(L"abc"), 0);
    CHECK_WSTR(dr, L""); CHECK_WSTR(di, L""); CHECK_WSTR(fn, L"abc"); CHECK_WSTR(ex, L"");
}
TC_CASE(null_components)
{
    CHECK_EQ_I(_wsplitpath_s(L"C:\\a\\b.c", NULL, 0, NULL, 0, fn, _MAX_FNAME, ex, _MAX_EXT), 0);
    CHECK_WSTR(fn, L"b"); CHECK_WSTR(ex, L".c");
}
int main(void) { tc_begin(); TC_RUN(full_path); TC_RUN(no_drive); TC_RUN(filename_only); TC_RUN(null_components); return tc_end(); }
