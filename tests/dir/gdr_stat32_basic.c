/* TC
 * id:    gdr_stat32_basic
 * area:  dir
 * funcs: _stat32 _stat32i64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _stat, _wstat family (st_mode, st_size, st_nlink, st_dev/st_rdev drive number)
 */
#include "tc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <winioctl.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define F "tc_gdr_stat32_basic_f" TC_S(TC_CRT_VER) ".tmp"
#define DIR_ "tc_gdr_stat32_basic_d" TC_S(TC_CRT_VER) ".tmp"
static const char *volatile v_path;

static void mkfile(const char *p, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char c = 0x45; DWORD w; int i;
    if (h == INVALID_HANDLE_VALUE) return;
    for (i = 0; i < n; i++) WriteFile(h, &c, 1, &w, NULL);
    CloseHandle(h);
}
static void cleanup(void) { DeleteFileA(F); DeleteFileA(DIR_ "\\big.tmp"); RemoveDirectoryA(DIR_); }

TC_CASE(file)
{
    struct _stat32 st; int r;
    mkfile(F, 123); v_path = F;
    r = _stat32(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) {
        CHECK_EQ_I(st.st_size, 123);
        CHECK(st.st_mode & _S_IFREG); CHECK(!(st.st_mode & _S_IFDIR));
        CHECK_EQ_I(st.st_nlink, 1);
        CHECK((st.st_mode & (_S_IREAD | _S_IWRITE)) == (_S_IREAD | _S_IWRITE));
    }
    cleanup();
}
TC_CASE(dir)
{
    struct _stat32 st; int r;
    CreateDirectoryA(DIR_, NULL); v_path = DIR_;
    r = _stat32(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK(st.st_mode & _S_IFDIR); CHECK(!(st.st_mode & _S_IFREG)); CHECK(st.st_mode & _S_IREAD); }
    cleanup();
}
TC_CASE(drive_index)
{
    struct _stat32 st; char cwd[MAX_PATH]; int r, idx;
    DWORD n = GetCurrentDirectoryA(MAX_PATH, cwd);
    if (n < 3 || cwd[1] != ':') TC_SKIP("cwd is not drive-letter based");
    idx = (cwd[0] >= 'a' && cwd[0] <= 'z') ? cwd[0] - 'a' : cwd[0] - 'A';
    mkfile(F, 1); v_path = F;
    r = _stat32(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK_EQ_I(st.st_dev, idx); CHECK_EQ_I(st.st_rdev, idx); }
    cleanup();
}
TC_CASE(i64_matches)
{
    struct _stat32 a; struct _stat32i64 b; int r1, r2;
    mkfile(F, 77); v_path = F;
    r1 = _stat32(v_path, &a); r2 = _stat32i64(v_path, &b);
    CHECK_EQ_I(r1, 0); CHECK_EQ_I(r2, 0);
    if (!r1 && !r2) {
        CHECK_EQ_I(a.st_size, 77); CHECK_EQ_I(b.st_size, 77);
        CHECK_EQ_I(a.st_mode, b.st_mode); CHECK_EQ_I(a.st_mtime, b.st_mtime);
        CHECK_EQ_I(sizeof(b.st_size), 8);
    }
    cleanup();
}
TC_CASE(i64_sparse_5gb)
{
    struct _stat32i64 st; HANDLE f; DWORD n; LARGE_INTEGER li; int ok, r;
    CreateDirectoryA(DIR_, NULL);
    f = CreateFileA(DIR_ "\\big.tmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f == INVALID_HANDLE_VALUE) { cleanup(); TC_SKIP("cannot create file"); }
    DeviceIoControl(f, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &n, NULL);
    li.QuadPart = 5368709120LL;
    ok = SetFilePointerEx(f, li, NULL, FILE_BEGIN) && SetEndOfFile(f);
    CloseHandle(f);
    if (!ok) { cleanup(); TC_SKIP("filesystem refused 5GB file"); }
    v_path = DIR_ "\\big.tmp";
    r = _stat32i64(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) CHECK_EQ_I(st.st_size, 5368709120LL);
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(file); TC_RUN(dir); TC_RUN(drive_index); TC_RUN(i64_matches); TC_RUN(i64_sparse_5gb);
    cleanup();
    return tc_end();
}
