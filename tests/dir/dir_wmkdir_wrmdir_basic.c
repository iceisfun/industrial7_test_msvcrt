/* TC
 * id:    dir_wmkdir_wrmdir_basic
 * area:  dir
 * funcs: _wmkdir _wrmdir
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mkdir, _wmkdir; _rmdir, _wrmdir
 */
#include "tc.h"
#include <direct.h>
#include <wchar.h>

static int is_dirw(const wchar_t *p) { DWORD a = GetFileAttributesW(p); return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY); }
static int gonew(const wchar_t *p) { return GetFileAttributesW(p) == INVALID_FILE_ATTRIBUTES; }

static const wchar_t * volatile a1 = L"tc_dir_wmkdir_wrmdir_basic_a";
static const wchar_t * volatile u1 = L"tc_dir_wmkdir_wrmdir_basic_\u00e9\u4e2d";
static const wchar_t * volatile t1 = L"tc_dir_wmkdir_wrmdir_basic_t\\";
static const wchar_t * volatile t2 = L"tc_dir_wmkdir_wrmdir_basic_t2/";

TC_CASE(ascii)
{
    CHECK_EQ_I(_wmkdir(a1), 0);
    CHECK(is_dirw(a1));
    CHECK_EQ_I(_wrmdir(a1), 0);
    CHECK(gonew(a1));
}
TC_CASE(non_ascii)
{
    CHECK_EQ_I(_wmkdir(u1), 0);
    CHECK(is_dirw(u1));
    CHECK_EQ_I(_wrmdir(u1), 0);
    CHECK(gonew(u1));
}
TC_CASE(trailing_sep)
{
    CHECK_EQ_I(_wmkdir(t1), 0);
    CHECK(is_dirw(L"tc_dir_wmkdir_wrmdir_basic_t"));
    CHECK_EQ_I(_wrmdir(t1), 0);
    CHECK_EQ_I(_wmkdir(t2), 0);
    CHECK(is_dirw(L"tc_dir_wmkdir_wrmdir_basic_t2"));
    CHECK_EQ_I(_wrmdir(t2), 0);
    CHECK(gonew(L"tc_dir_wmkdir_wrmdir_basic_t2"));
}
int main(void)
{
    tc_begin();
    RemoveDirectoryW(L"tc_dir_wmkdir_wrmdir_basic_a"); RemoveDirectoryW(u1); RemoveDirectoryW(L"tc_dir_wmkdir_wrmdir_basic_t"); RemoveDirectoryW(L"tc_dir_wmkdir_wrmdir_basic_t2");
    TC_RUN(ascii); TC_RUN(non_ascii); TC_RUN(trailing_sep);
    return tc_end();
}
