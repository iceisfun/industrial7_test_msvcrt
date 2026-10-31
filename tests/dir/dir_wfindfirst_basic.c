/* TC
 * id:    dir_wfindfirst_basic
 * area:  dir
 * funcs: _findclose
 * procs: _wfindfirst64i32 _wfindnext64i32
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _wfindfirst, _wfindnext (_wfinddata64i32_t has wchar_t name[260])
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

#define TC_S2(x) L ## #x
#define TC_S(x) TC_S2(x)
#define WD L"tc_dir_wfindfirst_basic_d" TC_S(TC_CRT_VER) L".tmp"
typedef intptr_t (__cdecl *ff_t)(const wchar_t *, struct _wfinddata64i32_t *);
typedef int (__cdecl *fn_t)(intptr_t, struct _wfinddata64i32_t *);
static ff_t p_ff; static fn_t p_fn;
static const wchar_t *volatile v_spec;
static void mkw(const wchar_t *p, int n)
{
    HANDLE h = CreateFileW(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char c = 'x'; DWORD w; int i;
    if (h == INVALID_HANDLE_VALUE) return;
    for (i = 0; i < n; i++) WriteFile(h, &c, 1, &w, NULL);
    CloseHandle(h);
}
static void cleanup(void)
{
    DeleteFileW(WD L"\\a.txt"); DeleteFileW(WD L"\\\x00e9\x4e2d.txt"); DeleteFileW(WD L"\\c.dat"); RemoveDirectoryW(WD);
}
TC_CASE(layout)
{
    struct _wfinddata64i32_t f;
    CHECK_EQ_U(sizeof f.name, 260 * sizeof(wchar_t)); CHECK_EQ_U(sizeof f.size, 4); CHECK_EQ_U(sizeof f.time_write, 8);
}
TC_CASE(wildcard)
{
    struct _wfinddata64i32_t fd; intptr_t h; int n = 0, mask = 0, r;
    if (!p_ff || !p_fn) TC_SKIP("not exported");
    CreateDirectoryW(WD, NULL); mkw(WD L"\\a.txt", 7); mkw(WD L"\\\x00e9\x4e2d.txt", 3); mkw(WD L"\\c.dat", 1);
    v_spec = WD L"\\*.txt"; h = p_ff(v_spec, &fd);
    CHECK(h != -1);
    if (h == -1) return;
    do {
        n++;
        if (tc_wcsequ(fd.name, L"a.txt")) { mask |= 1; CHECK_EQ_U(fd.size, 7); }
        else if (tc_wcsequ(fd.name, L"\x00e9\x4e2d.txt")) { mask |= 2; CHECK_EQ_U(fd.size, 3); }
        else mask |= 4;
        errno = 0;
    } while ((r = p_fn(h, &fd)) == 0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(mask, 3);
    CHECK_EQ_I(_findclose(h), 0);
}
TC_CASE(no_match)
{
    struct _wfinddata64i32_t fd; intptr_t h;
    if (!p_ff) TC_SKIP("not exported");
    v_spec = WD L"\\*.zzz"; errno = 0; h = p_ff(v_spec, &fd);
    CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, ENOENT);
    if (h != -1) _findclose(h);
}
int main(void)
{
    tc_begin(); cleanup();
    p_ff = (ff_t)tc_crt_proc("_wfindfirst64i32"); p_fn = (fn_t)tc_crt_proc("_wfindnext64i32");
    TC_RUN(layout); TC_RUN(wildcard); TC_RUN(no_match);
    cleanup();
    return tc_end();
}
