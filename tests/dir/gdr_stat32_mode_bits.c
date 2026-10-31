/* TC
 * id:    gdr_stat32_mode_bits
 * area:  dir
 * funcs: _stat32 _wstat32i64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _stat st_mode (_S_IREAD, _S_IWRITE; read-only attribute removes write permission)
 */
#include "tc.h"
#include <sys/types.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define W2(x) L##x
#define W(x) W2(x)
#define F "tc_gdr_stat32_mode_f" TC_S(TC_CRT_VER) ".tmp"
#define DIR_ "tc_gdr_stat32_mode_d" TC_S(TC_CRT_VER) ".tmp"
static const char *volatile v_path;
static const wchar_t *volatile v_wpath;

static void mkfile(const char *p)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}
static void cleanup(void)
{
    SetFileAttributesA(F, FILE_ATTRIBUTE_NORMAL); DeleteFileA(F); RemoveDirectoryA(DIR_);
}

TC_CASE(readonly_file)
{
    struct _stat32 st; struct _stat32i64 st2; int r;
    mkfile(F); SetFileAttributesA(F, FILE_ATTRIBUTE_READONLY); v_path = F;
    r = _stat32(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK(st.st_mode & _S_IREAD); CHECK(!(st.st_mode & _S_IWRITE)); }
    v_wpath = W(F);
    r = _wstat32i64(v_wpath, &st2);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK(st2.st_mode & _S_IREAD); CHECK(!(st2.st_mode & _S_IWRITE)); }
    cleanup();
}
TC_CASE(cleared_readonly)
{
    struct _stat32 st; struct _stat32i64 st2; int r;
    mkfile(F); SetFileAttributesA(F, FILE_ATTRIBUTE_READONLY); SetFileAttributesA(F, FILE_ATTRIBUTE_NORMAL);
    v_path = F; v_wpath = W(F);
    r = _stat32(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) CHECK((st.st_mode & (_S_IREAD | _S_IWRITE)) == (_S_IREAD | _S_IWRITE));
    r = _wstat32i64(v_wpath, &st2);
    CHECK_EQ_I(r, 0);
    if (r == 0) CHECK((st2.st_mode & (_S_IREAD | _S_IWRITE)) == (_S_IREAD | _S_IWRITE));
    cleanup();
}
TC_CASE(writable_dir)
{
    struct _stat32 st; int r;
    CreateDirectoryA(DIR_, NULL); v_path = DIR_;
    r = _stat32(v_path, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK(st.st_mode & _S_IFDIR); CHECK(st.st_mode & _S_IREAD); CHECK(st.st_mode & _S_IWRITE); }
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(readonly_file); TC_RUN(cleared_readonly); TC_RUN(writable_dir);
    cleanup();
    return tc_end();
}
