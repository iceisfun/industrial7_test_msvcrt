/* TC
 * id:    gdr_findfirst_plain_win7
 * area:  dir
 * funcs: _findclose
 * procs: _findfirst _findnext
 * crt:   win7
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _findfirst, _findnext (wildcards * and ?; -1 and ENOENT when no more matches)
 * note:  PROBE: besides the documented wildcard/ENOENT behaviour, this relies on the hand-typed plain _finddata_t layout (time_t is
 *        assumed 64-bit on x64 for the un-suffixed export of every non-UCRT CRT, 32-bit on x86) and on the un-suffixed export
 *        being that layout; a wrong layout would show as bad name/size. Plain _findfirst/_findnext are resolved at run time; where a CRT does not export the un-suffixed name the cases SKIP
 *        (msvcrt.dll is expected to export them). Hand-typed struct: attrib, three times (time_t width of the CRT's
 *        default: 64-bit on x64, 32-bit on x86), size (32-bit), name[260]. Only attrib/size/name are asserted.
 */
#include "tc.h"
#include <io.h>
#include <string.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define D "tc_gdr_findfirst_plain_d" TC_S(TC_CRT_VER) ".tmp"
#ifdef _WIN64
typedef long long tc_time;
#else
typedef long tc_time;
#endif
struct fdata { unsigned attrib; tc_time tc, ta, tw; unsigned size; char name[260]; };
typedef intptr_t (__cdecl *ff_t)(const char *, struct fdata *);
typedef int (__cdecl *fn_t)(intptr_t, struct fdata *);
static ff_t p_ff; static fn_t p_fn;
static const char *volatile v_spec;

static void mkfile(const char *p, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char c = 0x41; DWORD w; int i;
    if (h == INVALID_HANDLE_VALUE) return;
    for (i = 0; i < n; i++) WriteFile(h, &c, 1, &w, NULL);
    CloseHandle(h);
}
static void setup(void)
{
    CreateDirectoryA(D, NULL);
    mkfile(D "\\abc.tmp", 7); mkfile(D "\\aXc.tmp", 9); mkfile(D "\\abbc.tmp", 2);
}
static void cleanup(void)
{
    DeleteFileA(D "\\abc.tmp"); DeleteFileA(D "\\aXc.tmp"); DeleteFileA(D "\\abbc.tmp"); RemoveDirectoryA(D);
}
static int count(const char *spec, int *mask)
{
    struct fdata fd; intptr_t h; int n = 0, r; *mask = 0; v_spec = spec;
    memset(&fd, 0, sizeof fd);
    h = p_ff(v_spec, &fd);
    if (h == -1) return 0;
    do {
        n++;
        if (!strcmp(fd.name, "abc.tmp")) *mask |= 1; else if (!strcmp(fd.name, "aXc.tmp")) *mask |= 2;
        else if (!strcmp(fd.name, "abbc.tmp")) *mask |= 4; else *mask |= 8;
    } while ((r = p_fn(h, &fd)) == 0);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(_findclose(h), 0);
    return n;
}

TC_CASE(exported)
{
    if (TC_AT_LEAST(80) && !p_ff) TC_SKIP("plain _findfirst not exported by this CRT");
    CHECK(p_ff != NULL); CHECK(p_fn != NULL);
}
TC_CASE(star_tmp_two)
{
    int m, n;
    if (!p_ff || !p_fn) TC_SKIP("not exported");
    setup(); n = count(D "\\a?c.tmp", &m);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(m, 3);
    cleanup();
}
TC_CASE(star_all_three)
{
    int m, n;
    if (!p_ff || !p_fn) TC_SKIP("not exported");
    setup(); n = count(D "\\a*c.tmp", &m);
    CHECK_EQ_I(n, 3); CHECK_EQ_I(m, 7);
    cleanup();
}
TC_CASE(exact)
{
    struct fdata fd; intptr_t h; int r;
    if (!p_ff || !p_fn) TC_SKIP("not exported");
    setup(); v_spec = D "\\abc.tmp";
    h = p_ff(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        CHECK_STR(fd.name, "abc.tmp"); CHECK_EQ_U(fd.size, 7); CHECK(fd.attrib & _A_ARCH);
        errno = 0; r = p_fn(h, &fd); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
        CHECK_EQ_I(_findclose(h), 0);
    }
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    p_ff = (ff_t)tc_crt_proc("_findfirst"); p_fn = (fn_t)tc_crt_proc("_findnext");
    TC_RUN(exported); TC_RUN(star_tmp_two); TC_RUN(star_all_three); TC_RUN(exact);
    cleanup();
    return tc_end();
}
