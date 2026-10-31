/* TC
 * id:    gdr_wstat32_basic
 * area:  dir
 * funcs: _wstat32 _wstat32i64 _stat32
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _stat, _wstat family
 */
#include "tc.h"
#include <sys/types.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define W2(x) L##x
#define W(x) W2(x)
#define A_F "tc_gdr_wstat32_a" TC_S(TC_CRT_VER) ".tmp"
#define UF W(TC_S(TC_CRT_VER)) L"_\x00e9\x4e2d.tmp"
#define UD W(TC_S(TC_CRT_VER)) L"_d\x00e9\x4e2d.tmp"
static const wchar_t *volatile v_wpath;
static const char *volatile v_path;

static void mkfile(const wchar_t *p, int n)
{
    HANDLE h = CreateFileW(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char c = 0x46; DWORD w; int i;
    if (h == INVALID_HANDLE_VALUE) return;
    for (i = 0; i < n; i++) WriteFile(h, &c, 1, &w, NULL);
    CloseHandle(h);
}
static void cleanup(void) { DeleteFileW(UF); RemoveDirectoryW(UD); DeleteFileA(A_F); }

TC_CASE(unicode_file)
{
    struct _stat32 st; int r;
    mkfile(UF, 42); v_wpath = UF;
    r = _wstat32(v_wpath, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK_EQ_I(st.st_size, 42); CHECK(st.st_mode & _S_IFREG); CHECK(!(st.st_mode & _S_IFDIR)); }
    cleanup();
}
TC_CASE(i32_vs_i64)
{
    struct _stat32 a; struct _stat32i64 b; int r1, r2;
    mkfile(UF, 99); v_wpath = UF;
    r1 = _wstat32(v_wpath, &a); r2 = _wstat32i64(v_wpath, &b);
    CHECK_EQ_I(r1, 0); CHECK_EQ_I(r2, 0);
    if (!r1 && !r2) { CHECK_EQ_I(a.st_mode, b.st_mode); CHECK_EQ_I(a.st_size, 99); CHECK_EQ_I(b.st_size, 99); CHECK_EQ_I(a.st_mtime, b.st_mtime); }
    cleanup();
}
TC_CASE(unicode_dir)
{
    struct _stat32 st; int r;
    CreateDirectoryW(UD, NULL); v_wpath = UD;
    r = _wstat32(v_wpath, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK(st.st_mode & _S_IFDIR); CHECK(!(st.st_mode & _S_IFREG)); }
    cleanup();
}
TC_CASE(narrow_agrees)
{
    struct _stat32 a, b; int r1, r2; HANDLE h; DWORD w;
    h = CreateFileA(A_F, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) TC_SKIP("cannot create file");
    WriteFile(h, "12345", 5, &w, NULL); CloseHandle(h);
    v_path = A_F;
    {
        wchar_t full[64]; int i = 0, k = 0;
        const char *s = A_F;
        while (s[i]) full[k++] = (wchar_t)(unsigned char)s[i++];
        full[k] = 0; v_wpath = full;
        r1 = _stat32(v_path, &a); r2 = _wstat32(v_wpath, &b);
    }
    CHECK_EQ_I(r1, 0); CHECK_EQ_I(r2, 0);
    if (!r1 && !r2) {
        CHECK_EQ_I(a.st_mode, b.st_mode); CHECK_EQ_I(a.st_size, b.st_size); CHECK_EQ_I(a.st_size, 5);
        CHECK_EQ_I(a.st_mtime, b.st_mtime); CHECK_EQ_I(a.st_ctime, b.st_ctime); CHECK_EQ_I(a.st_atime, b.st_atime);
        CHECK_EQ_I(a.st_dev, b.st_dev);
    }
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(unicode_file); TC_RUN(i32_vs_i64); TC_RUN(unicode_dir); TC_RUN(narrow_agrees);
    cleanup();
    return tc_end();
}
