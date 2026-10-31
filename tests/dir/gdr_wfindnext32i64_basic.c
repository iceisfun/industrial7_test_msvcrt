/* TC
 * id:    gdr_wfindnext32i64_basic
 * area:  dir
 * funcs: _wfindfirst32i64 _wfindnext32i64 _findclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _wfindfirst, _wfindnext (struct _wfinddata32i64_t: 32-bit time, __int64 size)
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <winioctl.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define DN "tc_gdr_wfind32i64_d" TC_S(TC_CRT_VER) ".tmp"
#define W2(x) L##x
#define W(x) W2(x)
#define WD W(DN)
static const wchar_t *volatile v_spec;

static void mkfile(const wchar_t *p, int n)
{
    HANDLE h = CreateFileW(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char c = 0x43; DWORD w; int i;
    if (h == INVALID_HANDLE_VALUE) return;
    for (i = 0; i < n; i++) WriteFile(h, &c, 1, &w, NULL);
    CloseHandle(h);
}
static void cleanup(void)
{
    DeleteFileW(WD L"\\one.i64"); DeleteFileW(WD L"\\two.i64"); DeleteFileW(WD L"\\big.i64");
    RemoveDirectoryW(WD);
}

TC_CASE(small_size)
{
    struct _wfinddata32i64_t fd; intptr_t h;
    CreateDirectoryW(WD, NULL); mkfile(WD L"\\one.i64", 123);
    v_spec = WD L"\\one.i64";
    h = _wfindfirst32i64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        CHECK_WSTR(fd.name, L"one.i64"); CHECK_EQ_I(fd.size, 123); CHECK(fd.attrib & _A_ARCH);
        CHECK_EQ_I(sizeof(fd.size), 8);
        errno = 0; CHECK_EQ_I(_wfindnext32i64(h, &fd), -1); CHECK_EQ_I(errno, ENOENT);
        CHECK_EQ_I(_findclose(h), 0);
    }
    cleanup();
}
TC_CASE(big_sparse)
{
    struct _wfinddata32i64_t fd; intptr_t h; HANDLE f; DWORD n; LARGE_INTEGER li; int ok;
    CreateDirectoryW(WD, NULL);
    f = CreateFileW(WD L"\\big.i64", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f == INVALID_HANDLE_VALUE) { cleanup(); TC_SKIP("cannot create file"); }
    DeviceIoControl(f, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &n, NULL);
    li.QuadPart = 5368709120LL;
    ok = SetFilePointerEx(f, li, NULL, FILE_BEGIN) && SetEndOfFile(f);
    CloseHandle(f);
    if (!ok) { cleanup(); TC_SKIP("filesystem refused 5GB file"); }
    v_spec = WD L"\\big.i64";
    h = _wfindfirst32i64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_EQ_I(fd.size, 5368709120LL); _findclose(h); }
    cleanup();
}
TC_CASE(two_files)
{
    struct _wfinddata32i64_t fd; intptr_t h; int seen = 0, n = 0, r;
    CreateDirectoryW(WD, NULL); mkfile(WD L"\\one.i64", 1); mkfile(WD L"\\two.i64", 2);
    v_spec = WD L"\\*.i64";
    h = _wfindfirst32i64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        do {
            n++;
            if (tc_wcsequ(fd.name, L"one.i64")) { seen |= 1; CHECK_EQ_I(fd.size, 1); }
            else if (tc_wcsequ(fd.name, L"two.i64")) { seen |= 2; CHECK_EQ_I(fd.size, 2); }
            else seen |= 4;
        } while ((r = _wfindnext32i64(h, &fd)) == 0);
        CHECK_EQ_I(r, -1); CHECK_EQ_I(n, 2); CHECK_EQ_I(seen, 3);
        CHECK_EQ_I(_findclose(h), 0);
    }
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(small_size); TC_RUN(big_sparse); TC_RUN(two_files);
    cleanup();
    return tc_end();
}
