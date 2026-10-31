/* TC
 * id:    gdr_wfindnext32_basic
 * area:  dir
 * funcs: _wfindfirst32 _wfindnext32 _findclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _wfindfirst, _wfindnext (struct _wfinddata32_t); _findclose
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define DN "tc_gdr_wfind32_d" TC_S(TC_CRT_VER) ".tmp"
#define W2(x) L##x
#define W(x) W2(x)
#define WD W(DN)
static const wchar_t *volatile v_spec;
static const wchar_t UNI[] = L"u\x00e9\x4e2d.w32";

static void mkfile(const wchar_t *p, int n, DWORD attr)
{
    HANDLE h = CreateFileW(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, attr, NULL);
    char c = 0x42; DWORD w; int i;
    if (h == INVALID_HANDLE_VALUE) return;
    for (i = 0; i < n; i++) WriteFile(h, &c, 1, &w, NULL);
    CloseHandle(h);
}
static void cleanup(void)
{
    SetFileAttributesW(WD L"\\hid.w32", FILE_ATTRIBUTE_NORMAL);
    DeleteFileW(WD L"\\u\x00e9\x4e2d.w32"); DeleteFileW(WD L"\\plain.w32"); DeleteFileW(WD L"\\hid.w32");
    RemoveDirectoryW(WD);
}
static void setup(void)
{
    CreateDirectoryW(WD, NULL);
    mkfile(WD L"\\u\x00e9\x4e2d.w32", 11, FILE_ATTRIBUTE_NORMAL);
    mkfile(WD L"\\plain.w32", 4, FILE_ATTRIBUTE_NORMAL);
}

TC_CASE(unicode_name)
{
    struct _wfinddata32_t fd; intptr_t h;
    setup(); v_spec = WD L"\\u*.w32";
    h = _wfindfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        CHECK_WSTR(fd.name, UNI); CHECK_EQ_U(fd.size, 11);
        CHECK(fd.attrib & _A_ARCH);
        CHECK_EQ_I(_findclose(h), 0);
    }
    cleanup();
}
TC_CASE(two_then_end)
{
    struct _wfinddata32_t fd; intptr_t h; int n = 0, a = 0, b = 0, r;
    setup(); v_spec = WD L"\\*.w32";
    h = _wfindfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        do {
            n++;
            if (tc_wcsequ(fd.name, UNI)) a++; else if (tc_wcsequ(fd.name, L"plain.w32")) b++;
            errno = 0;
        } while ((r = _wfindnext32(h, &fd)) == 0);
        CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
        CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 1);
        r = _wfindnext32(h, &fd); CHECK_EQ_I(r, -1);
        CHECK_EQ_I(_findclose(h), 0);
    }
    cleanup();
}
TC_CASE(size_attrib)
{
    struct _wfinddata32_t fd; intptr_t h;
    setup(); v_spec = WD L"\\plain.w32";
    h = _wfindfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        CHECK_WSTR(fd.name, L"plain.w32"); CHECK_EQ_U(fd.size, 4);
        CHECK(!(fd.attrib & _A_SUBDIR)); CHECK(!(fd.attrib & _A_HIDDEN));
        _findclose(h);
    }
    cleanup();
}
TC_CASE(hidden)
{
    struct _wfinddata32_t fd; intptr_t h;
    setup(); mkfile(WD L"\\hid.w32", 2, FILE_ATTRIBUTE_NORMAL);
    SetFileAttributesW(WD L"\\hid.w32", FILE_ATTRIBUTE_HIDDEN);
    v_spec = WD L"\\hid.w32";
    h = _wfindfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_WSTR(fd.name, L"hid.w32"); CHECK(fd.attrib & _A_HIDDEN); CHECK_EQ_U(fd.size, 2); _findclose(h); }
    cleanup();
}
TC_CASE(close_zero)
{
    struct _wfinddata32_t fd; intptr_t h;
    setup(); v_spec = WD L"\\plain.w32";
    h = _wfindfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) CHECK_EQ_I(_findclose(h), 0);
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(unicode_name); TC_RUN(two_then_end); TC_RUN(size_attrib); TC_RUN(hidden); TC_RUN(close_zero);
    cleanup();
    return tc_end();
}
