/* TC
 * id:    gdr_neg_probe_chdir_rmdir_on_file
 * area:  dir
 * kind:  negative
 * funcs: _chdir _rmdir
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _chdir, _rmdir (-1 on failure with errno set)
 * note:  PROBE: _chdir and _rmdir applied to an existing regular FILE fail with -1, set a non-zero errno, leave the file in place and the cwd
 *        unchanged. The exact errno is not asserted.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>
#include <string.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define F "tc_gdr_neg_chdir_file" TC_S(TC_CRT_VER) ".tmp"
static const char *volatile v_path;

static void cleanup(void) { DeleteFileA(F); }
static int mk(void)
{
    HANDLE h = CreateFileA(F, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0;
    CloseHandle(h); return 1;
}
TC_CASE(chdir_file)
{
    char before[MAX_PATH], after[MAX_PATH];
    if (!mk()) TC_SKIP("cannot create file");
    GetCurrentDirectoryA(MAX_PATH, before);
    v_path = F; errno = 0;
    CHECK_EQ_I(_chdir(v_path), -1); CHECK(errno != 0);
    GetCurrentDirectoryA(MAX_PATH, after);
    CHECK_STR(after, before);
    cleanup();
}
TC_CASE(rmdir_file)
{
    if (!mk()) TC_SKIP("cannot create file");
    v_path = F; errno = 0;
    CHECK_EQ_I(_rmdir(v_path), -1); CHECK(errno != 0);
    CHECK(GetFileAttributesA(F) != INVALID_FILE_ATTRIBUTES);
    cleanup();
}
int main(void) { tc_begin(); cleanup(); TC_RUN(chdir_file); TC_RUN(rmdir_file); cleanup(); return tc_end(); }
