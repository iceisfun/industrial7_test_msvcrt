/* TC
 * id:    dir_findfirst_basic
 * area:  dir
 * funcs: _findclose
 * procs: _findfirst64i32 _findnext64i32
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _findfirst, _findnext, _findclose (wildcards * and ?, returns 0 on success, _A_ARCH/_A_SUBDIR attrib)
 * note:  on msvcrt.dll (60) a missing _findfirst64i32/_findnext64i32 export skips the whole test (not known to exist there).
 *        _findfirst64i32/_findnext64i32 are inline wrappers in mingw headers, so they are resolved from the DLL at run time.
 */
#include "tc.h"
#include <io.h>
#include <string.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define D "tc_dir_findfirst_basic_d" TC_S(TC_CRT_VER) ".tmp"
typedef intptr_t (__cdecl *ff_t)(const char *, struct _finddata64i32_t *);
typedef int (__cdecl *fn_t)(intptr_t, struct _finddata64i32_t *);
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
    CreateDirectoryA(D, NULL); CreateDirectoryA(D "\\sub.d", NULL);
    mkfile(D "\\a.txt", 10); mkfile(D "\\b.txt", 0); mkfile(D "\\c.dat", 25);
}
static void cleanup(void)
{
    DeleteFileA(D "\\a.txt"); DeleteFileA(D "\\b.txt"); DeleteFileA(D "\\c.dat");
    RemoveDirectoryA(D "\\sub.d"); RemoveDirectoryA(D);
}
/* enumerate; returns count of entries; sets bitmask: 1=a.txt 2=b.txt 4=c.dat 8=sub.d 16=other */
static int enumerate(const char *spec, int *mask, int *nonexh_errno)
{
    struct _finddata64i32_t fd; intptr_t h; int n = 0, r; *mask = 0; v_spec = spec;
    h = p_ff(v_spec, &fd);
    if (h == -1) return -1;
    do {
        n++;
        if (!strcmp(fd.name, "a.txt")) *mask |= 1; else if (!strcmp(fd.name, "b.txt")) *mask |= 2;
        else if (!strcmp(fd.name, "c.dat")) *mask |= 4; else if (!strcmp(fd.name, "sub.d")) *mask |= 8;
        else if (strcmp(fd.name, ".") && strcmp(fd.name, "..")) *mask |= 16;
        errno = 0;
    } while ((r = p_fn(h, &fd)) == 0);
    *nonexh_errno = errno;
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(_findclose(h), 0);
    return n;
}

TC_CASE(exported)
{
    /* the 64i32 variants are certain exports from msvcr80 on; msvcrt.dll (Win7) is not known to export them */
    if ((!p_ff || !p_fn) && TC_BELOW(80)) TC_SKIP("msvcrt.dll may not export _findfirst64i32");
    CHECK(p_ff != NULL); CHECK(p_fn != NULL);
}
TC_CASE(star_txt)
{
    int m, e = 0, n;
    if (!p_ff || !p_fn) TC_SKIP("not exported");
    setup(); n = enumerate(D "\\*.txt", &m, &e);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(m, 3); CHECK_EQ_I(e, ENOENT);
    cleanup();
}
TC_CASE(star_all)
{
    int m, e = 0, n;
    if (!p_ff || !p_fn) TC_SKIP("not exported");
    setup(); n = enumerate(D "\\*", &m, &e);
    CHECK(n >= 4);  /* a.txt b.txt c.dat sub.d plus "." and ".." on NTFS */
    CHECK_EQ_I(m, 15);
    cleanup();
}
TC_CASE(question_mark)
{
    int m, e = 0, n;
    if (!p_ff || !p_fn) TC_SKIP("not exported");
    setup(); n = enumerate(D "\\?.dat", &m, &e);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(m, 4);
    cleanup();
}
TC_CASE(exact_name)
{
    int m, e = 0, n;
    if (!p_ff || !p_fn) TC_SKIP("not exported");
    setup(); n = enumerate(D "\\b.txt", &m, &e);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(m, 2); CHECK_EQ_I(e, ENOENT);
    cleanup();
}
TC_CASE(size_and_attrib)
{
    struct _finddata64i32_t fd; intptr_t h;
    if (!p_ff) TC_SKIP("not exported");
    setup(); v_spec = D "\\a.txt";
    h = p_ff(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        CHECK_STR(fd.name, "a.txt"); CHECK_EQ_U(fd.size, 10);
        CHECK(fd.attrib & _A_ARCH); CHECK(!(fd.attrib & _A_SUBDIR));
        CHECK_EQ_I(_findclose(h), 0);
    }
    v_spec = D "\\c.dat"; h = p_ff(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_EQ_U(fd.size, 25); _findclose(h); }
    cleanup();
}
TC_CASE(subdir_attrib)
{
    struct _finddata64i32_t fd; intptr_t h;
    if (!p_ff) TC_SKIP("not exported");
    setup(); v_spec = D "\\sub.d";
    h = p_ff(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_STR(fd.name, "sub.d"); CHECK(fd.attrib & _A_SUBDIR); _findclose(h); }
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    p_ff = (ff_t)tc_crt_proc("_findfirst64i32"); p_fn = (fn_t)tc_crt_proc("_findnext64i32");
    TC_RUN(exported); TC_RUN(star_txt); TC_RUN(star_all); TC_RUN(question_mark);
    TC_RUN(exact_name); TC_RUN(size_and_attrib); TC_RUN(subdir_attrib);
    cleanup();
    return tc_end();
}
