/* TC
 * id:    gdr_wfindnext64_basic
 * area:  dir
 * funcs: _wfindfirst64 _wfindnext64 _findclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _wfindfirst, _wfindnext (struct _wfinddata64_t: __time64_t times, __int64 size)
 * note:  time_create > 0 assumes an NTFS volume; the assertion is skipped on other file systems.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define DN "tc_gdr_wfind64_d" TC_S(TC_CRT_VER) ".tmp"
#define W2(x) L##x
#define W(x) W2(x)
#define WD W(DN)
static const wchar_t *volatile v_spec;

static void mkfile(const wchar_t *p, int n)
{
    HANDLE h = CreateFileW(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char c = 0x44; DWORD w; int i;
    if (h == INVALID_HANDLE_VALUE) return;
    for (i = 0; i < n; i++) WriteFile(h, &c, 1, &w, NULL);
    CloseHandle(h);
}
static void setup(void)
{
    CreateDirectoryW(WD, NULL); CreateDirectoryW(WD L"\\s64", NULL);
    mkfile(WD L"\\a.f64", 10); mkfile(WD L"\\b.f64", 0); mkfile(WD L"\\c.f64", 300);
}
static void cleanup(void)
{
    DeleteFileW(WD L"\\a.f64"); DeleteFileW(WD L"\\b.f64"); DeleteFileW(WD L"\\c.f64");
    RemoveDirectoryW(WD L"\\s64"); RemoveDirectoryW(WD);
}
static int is_ntfs(void)
{
    wchar_t fs[32]; fs[0] = 0;
    if (!GetVolumeInformationW(NULL, NULL, 0, NULL, NULL, NULL, fs, 32)) return 0;
    return fs[0] == 'N' && fs[1] == 'T' && fs[2] == 'F' && fs[3] == 'S' && !fs[4];
}

TC_CASE(enumerate_three)
{
    struct _wfinddata64_t fd; intptr_t h; int mask = 0, n = 0, r;
    setup(); v_spec = WD L"\\*.f64";
    h = _wfindfirst64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        do {
            n++;
            if (tc_wcsequ(fd.name, L"a.f64")) { mask |= 1; CHECK_EQ_I(fd.size, 10); }
            else if (tc_wcsequ(fd.name, L"b.f64")) { mask |= 2; CHECK_EQ_I(fd.size, 0); }
            else if (tc_wcsequ(fd.name, L"c.f64")) { mask |= 4; CHECK_EQ_I(fd.size, 300); }
            else mask |= 8;
        } while ((r = _wfindnext64(h, &fd)) == 0);
        CHECK_EQ_I(r, -1); CHECK_EQ_I(n, 3); CHECK_EQ_I(mask, 7);
        CHECK_EQ_I(_findclose(h), 0);
    }
    cleanup();
}
TC_CASE(times_positive)
{
    struct _wfinddata64_t fd; intptr_t h;
    setup(); v_spec = WD L"\\a.f64";
    h = _wfindfirst64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        CHECK(fd.time_write > 0);
        if (is_ntfs()) CHECK(fd.time_create > 0);
        _findclose(h);
    }
    cleanup();
}
TC_CASE(subdir)
{
    struct _wfinddata64_t fd; intptr_t h;
    setup(); v_spec = WD L"\\s6*";
    h = _wfindfirst64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_WSTR(fd.name, L"s64"); CHECK(fd.attrib & _A_SUBDIR); CHECK_EQ_I(_findclose(h), 0); }
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(enumerate_three); TC_RUN(times_positive); TC_RUN(subdir);
    cleanup();
    return tc_end();
}
