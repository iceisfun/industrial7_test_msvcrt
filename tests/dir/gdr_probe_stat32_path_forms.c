/* TC
 * id:    gdr_probe_stat32_path_forms
 * area:  dir
 * funcs: _stat32
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _stat (silent on trailing separators)
 * note:  PROBE: (1) _stat32("dir\\") on an existing directory succeeds with S_IFDIR; (2) the drive root "X:\\" of the cwd
 *        drive succeeds with S_IFDIR; (3) "file\\" (trailing backslash on a regular file) fails with -1.
 *        Old CRTs were known to reject trailing backslashes on non-root directories; msvcr80+ is believed to accept them.
 */
#include "tc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define F "tc_gdr_probe_pathforms_f" TC_S(TC_CRT_VER) ".tmp"
#define DIR_ "tc_gdr_probe_pathforms_d" TC_S(TC_CRT_VER) ".tmp"
static const char *volatile v_path;

static void cleanup(void) { DeleteFileA(F); RemoveDirectoryA(DIR_); }

TC_CASE(dir_trailing_backslash)
{
    struct _stat32 st; int r;
    CreateDirectoryA(DIR_, NULL); v_path = DIR_ "\\";
    r = _stat32(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) CHECK(st.st_mode & _S_IFDIR);
    cleanup();
}
TC_CASE(drive_root)
{
    struct _stat32 st; char cwd[MAX_PATH], root[4]; int r;
    if (GetCurrentDirectoryA(MAX_PATH, cwd) < 3 || cwd[1] != ':') TC_SKIP("cwd is not drive-letter based");
    root[0] = cwd[0]; root[1] = ':'; root[2] = '\\'; root[3] = 0; v_path = root;
    r = _stat32(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) CHECK(st.st_mode & _S_IFDIR);
}
TC_CASE(file_trailing_backslash)
{
    struct _stat32 st; HANDLE h; int r;
    h = CreateFileA(F, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) TC_SKIP("cannot create file");
    CloseHandle(h); v_path = F "\\";
    r = _stat32(v_path, &st);
    CHECK_EQ_I(r, -1);
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(dir_trailing_backslash); TC_RUN(drive_root); TC_RUN(file_trailing_backslash);
    cleanup();
    return tc_end();
}
