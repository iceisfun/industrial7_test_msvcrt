/* TC
 * id:    dir_splitpath_s_basic
 * area:  dir
 * funcs:
 * procs: _splitpath_s _wsplitpath_s
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _splitpath_s, _wsplitpath_s (NULL buffer permitted only together with size 0)
 * note:  Resolved via procs: msvcr80's mingw import lib lacks the _s names. msvcrt.dll (60) exports _splitpath_s only on
 *        newer Windows; crt:all follows the MS requirements table (msvcrt.dll since Vista).
 */
#include "tc.h"

typedef int (__cdecl *sp_t)(const char *, char *, size_t, char *, size_t, char *, size_t, char *, size_t);
typedef int (__cdecl *wsp_t)(const wchar_t *, wchar_t *, size_t, wchar_t *, size_t, wchar_t *, size_t, wchar_t *, size_t);
static sp_t sp; static wsp_t wsp;

static const char *vs(const char *s) { static volatile const char *v; v = s; return (const char *)v; }
static const wchar_t *vw(const wchar_t *s) { static volatile const wchar_t *v; v = s; return (const wchar_t *)v; }

static void chk(const char *path, const char *d, const char *dir, const char *f, const char *e)
{
    char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    CHECK_EQ_I(sp(vs(path), dr, _MAX_DRIVE, di, _MAX_DIR, fn, _MAX_FNAME, ex, _MAX_EXT), 0);
    CHECK_STR(dr, d); CHECK_STR(di, dir); CHECK_STR(fn, f); CHECK_STR(ex, e);
}
TC_CASE(vectors)
{
    if (!sp) TC_SKIP("_splitpath_s not exported");
    chk("C:\\dir\\sub\\file.ext", "C:", "\\dir\\sub\\", "file", ".ext");
    chk("file", "", "", "file", "");
    chk("dir\\", "", "dir\\", "", "");
    chk("C:file.txt", "C:", "", "file", ".txt");
    chk("C:\\", "C:", "\\", "", "");
    chk("a.b.c", "", "", "a.b", ".c");
    chk(".hidden", "", "", "", ".hidden");
    chk("file.", "", "", "file", ".");
}
TC_CASE(exact_sizes)
{
    char dr[3], di[10], fn[5], ex[5];
    if (!sp) TC_SKIP("_splitpath_s not exported");
    CHECK_EQ_I(sp(vs("C:\\dir\\sub\\file.ext"), dr, 3, di, 10, fn, 5, ex, 5), 0);
    CHECK_STR(dr, "C:"); CHECK_STR(di, "\\dir\\sub\\"); CHECK_STR(fn, "file"); CHECK_STR(ex, ".ext");
}
TC_CASE(null_size_zero_pairs)
{
    char fn[_MAX_FNAME], ex[_MAX_EXT];
    if (!sp) TC_SKIP("_splitpath_s not exported");
    CHECK_EQ_I(sp(vs("C:\\d\\f.e"), NULL, 0, NULL, 0, fn, _MAX_FNAME, ex, _MAX_EXT), 0);
    CHECK_STR(fn, "f"); CHECK_STR(ex, ".e");
}
TC_CASE(empty_path)
{
    char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    dr[0] = di[0] = fn[0] = ex[0] = 'X';
    if (!sp) TC_SKIP("_splitpath_s not exported");
    CHECK_EQ_I(sp(vs(""), dr, _MAX_DRIVE, di, _MAX_DIR, fn, _MAX_FNAME, ex, _MAX_EXT), 0);
    CHECK_STR(dr, ""); CHECK_STR(di, ""); CHECK_STR(fn, ""); CHECK_STR(ex, "");
}
TC_CASE(wide)
{
    wchar_t dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    wchar_t d2[3], i2[6], f2[5], e2[5];
    if (!wsp) TC_SKIP("_wsplitpath_s not exported");
    CHECK_EQ_I(wsp(vw(L"C:\\dir\\caf\x00e9.ext"), dr, _MAX_DRIVE, di, _MAX_DIR, fn, _MAX_FNAME, ex, _MAX_EXT), 0);
    CHECK_WSTR(dr, L"C:"); CHECK_WSTR(di, L"\\dir\\"); CHECK_WSTR(fn, L"caf\x00e9"); CHECK_WSTR(ex, L".ext");
    CHECK_EQ_I(wsp(vw(L"C:\\dir\\file.ext"), d2, 3, i2, 6, f2, 5, e2, 5), 0);
    CHECK_WSTR(d2, L"C:"); CHECK_WSTR(i2, L"\\dir\\"); CHECK_WSTR(f2, L"file"); CHECK_WSTR(e2, L".ext");
    CHECK_EQ_I(wsp(vw(L""), dr, _MAX_DRIVE, di, _MAX_DIR, fn, _MAX_FNAME, ex, _MAX_EXT), 0);
    CHECK_WSTR(dr, L""); CHECK_WSTR(fn, L"");
}
int main(void)
{
    tc_begin();
    sp = (sp_t)tc_crt_proc("_splitpath_s"); wsp = (wsp_t)tc_crt_proc("_wsplitpath_s");
    TC_RUN(vectors); TC_RUN(exact_sizes); TC_RUN(null_size_zero_pairs); TC_RUN(empty_path); TC_RUN(wide);
    return tc_end();
}
