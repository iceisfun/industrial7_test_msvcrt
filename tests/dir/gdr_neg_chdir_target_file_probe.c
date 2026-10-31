/* TC
 * id:    gdr_neg_chdir_target_file_probe
 * area:  dir
 * kind:  negative
 * funcs: _chdir
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _chdir (returns -1 on failure, errno ENOENT)
 * note:  PROBE: _chdir to a regular file, and to a path with a 300-char component, returns -1 and leaves cwd unchanged
 *        (certain); errno only checked as a member of {ENOENT,ENOTDIR,EINVAL} resp. {ENOENT,ENAMETOOLONG}.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define FN "tc_gdr_chdir_file" TC_S(TC_CRT_VER) ".tmp"
static const char *volatile v_p;

TC_CASE(file_target)
{
    char before[MAX_PATH], after[MAX_PATH]; int r;
    HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    GetCurrentDirectoryA(MAX_PATH, before);
    v_p = FN; errno = 0; r = _chdir(v_p);
    CHECK_EQ_I(r, -1);
    if (r == -1) CHECK(errno == ENOENT || errno == ENOTDIR || errno == EINVAL);
    GetCurrentDirectoryA(MAX_PATH, after);
    CHECK_STR(after, before);
    DeleteFileA(FN);
}
TC_CASE(overlong_component)
{
    char before[MAX_PATH], after[MAX_PATH], p[320]; int i, r;
    for (i = 0; i < 300; i++) p[i] = 'a';
    p[300] = 0;
    GetCurrentDirectoryA(MAX_PATH, before);
    v_p = p; errno = 0; r = _chdir(v_p);
    CHECK_EQ_I(r, -1);
    if (r == -1) CHECK(errno == ENOENT || errno == ENAMETOOLONG);
    GetCurrentDirectoryA(MAX_PATH, after);
    CHECK_STR(after, before);
}
int main(void) { tc_begin(); DeleteFileA(FN); TC_RUN(file_target); TC_RUN(overlong_component); DeleteFileA(FN); return tc_end(); }
