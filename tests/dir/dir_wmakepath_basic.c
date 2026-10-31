/* TC
 * id:    dir_wmakepath_basic
 * area:  dir
 * funcs: _wmakepath
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _makepath, _wmakepath
 */
#include "tc.h"
#include <stdlib.h>

static const wchar_t *volatile v_drive, *volatile v_dir, *volatile v_fname, *volatile v_ext;
static void mk(wchar_t *b, const wchar_t *d, const wchar_t *dir, const wchar_t *f, const wchar_t *e)
{ v_drive = d; v_dir = dir; v_fname = f; v_ext = e; _wmakepath(b, v_drive, v_dir, v_fname, v_ext); }

TC_CASE(full)       { wchar_t b[260]; mk(b, L"C:", L"\\dir\\sub\\", L"name", L".ext"); CHECK_WSTR(b, L"C:\\dir\\sub\\name.ext"); }
TC_CASE(inserted)   { wchar_t b[260]; mk(b, L"C", L"\\dir", L"name", L"ext"); CHECK_WSTR(b, L"C:\\dir\\name.ext"); }
TC_CASE(skipped)
{
    wchar_t b[260];
    mk(b, NULL, NULL, L"name", NULL); CHECK_WSTR(b, L"name");
    mk(b, L"", L"", L"name", L""); CHECK_WSTR(b, L"name");
    mk(b, NULL, NULL, NULL, NULL); CHECK_WSTR(b, L"");
}
TC_CASE(non_ascii)
{
    wchar_t b[260];
    mk(b, L"C:", L"\x00e9t\x00e9\\", L"\x00fc\x4e2d\x6587", L".txt");
    CHECK_WSTR(b, L"C:\x00e9t\x00e9\\\x00fc\x4e2d\x6587.txt");
}
int main(void) { tc_begin(); TC_RUN(full); TC_RUN(inserted); TC_RUN(skipped); TC_RUN(non_ascii); return tc_end(); }
