/* TC
 * id:    gdr_findfirst32_findnext32_basic
 * area:  dir
 * funcs: _findfirst32 _findnext32 _findclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _findfirst, _findnext, _findclose (struct _finddata32_t: 32-bit time, 32-bit size)
 * note:  ucrtbase exports _findfirst32 but not plain _findfirst; msvcr80+ export the explicit-width names.
 */
#include "tc.h"
#include <io.h>
#include <string.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define D "tc_gdr_findfirst32_basic_d" TC_S(TC_CRT_VER) ".tmp"
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
    mkfile(D "\\a.dat", 17); mkfile(D "\\b.dat", 5); mkfile(D "\\c.txt", 3);
}
static void cleanup(void)
{
    DeleteFileA(D "\\a.dat"); DeleteFileA(D "\\b.dat"); DeleteFileA(D "\\c.txt");
    RemoveDirectoryA(D "\\sub.d"); RemoveDirectoryA(D);
}

TC_CASE(two_dat)
{
    struct _finddata32_t fd; intptr_t h; int n = 0, seen_a = 0, seen_b = 0, r;
    setup(); v_spec = D "\\*.dat";
    h = _findfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        do {
            n++;
            if (!strcmp(fd.name, "a.dat")) seen_a++; else if (!strcmp(fd.name, "b.dat")) seen_b++;
            CHECK(strchr(fd.name, '\\') == NULL && strchr(fd.name, '/') == NULL);
            errno = 0;
        } while ((r = _findnext32(h, &fd)) == 0);
        CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
        CHECK_EQ_I(n, 2); CHECK_EQ_I(seen_a, 1); CHECK_EQ_I(seen_b, 1);
        CHECK_EQ_I(_findclose(h), 0);
    }
    cleanup();
}
TC_CASE(size_attrib)
{
    struct _finddata32_t fd; intptr_t h;
    setup(); v_spec = D "\\a.dat";
    h = _findfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        CHECK_STR(fd.name, "a.dat"); CHECK_EQ_U(fd.size, 17);
        CHECK(fd.attrib & _A_ARCH); CHECK(!(fd.attrib & _A_SUBDIR));
        CHECK(fd.time_write > 0);
        CHECK_EQ_I(_findclose(h), 0);
    }
    v_spec = D "\\b.dat"; h = _findfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_EQ_U(fd.size, 5); _findclose(h); }
    cleanup();
}
TC_CASE(subdir)
{
    struct _finddata32_t fd; intptr_t h; int n = 0, r;
    setup(); v_spec = D "\\sub*";
    h = _findfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        CHECK_STR(fd.name, "sub.d"); CHECK(fd.attrib & _A_SUBDIR);
        n = 1;
        r = _findnext32(h, &fd); CHECK_EQ_I(r, -1);
        CHECK_EQ_I(_findclose(h), 0);
    }
    CHECK_EQ_I(n, 1);
    cleanup();
}
TC_CASE(star_all_unique)
{
    struct _finddata32_t fd; intptr_t h; int na = 0, nb = 0, nc = 0, ns = 0, other = 0, dots = 0, r;
    setup(); v_spec = D "\\*";
    h = _findfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) {
        do {
            if (!strcmp(fd.name, "a.dat")) na++; else if (!strcmp(fd.name, "b.dat")) nb++;
            else if (!strcmp(fd.name, "c.txt")) nc++; else if (!strcmp(fd.name, "sub.d")) ns++;
            else if (!strcmp(fd.name, ".") || !strcmp(fd.name, "..")) dots++;
            else other++;
        } while ((r = _findnext32(h, &fd)) == 0);
        CHECK_EQ_I(r, -1);
        CHECK_EQ_I(_findclose(h), 0);
    }
    CHECK_EQ_I(na, 1); CHECK_EQ_I(nb, 1); CHECK_EQ_I(nc, 1); CHECK_EQ_I(ns, 1); CHECK_EQ_I(other, 0);
    CHECK(dots <= 2);
    cleanup();
}
TC_CASE(close_zero)
{
    struct _finddata32_t fd; intptr_t h;
    setup(); v_spec = D "\\c.txt";
    h = _findfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) CHECK_EQ_I(_findclose(h), 0);
    cleanup();
}
int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(two_dat); TC_RUN(size_attrib); TC_RUN(subdir); TC_RUN(star_all_unique); TC_RUN(close_zero);
    cleanup();
    return tc_end();
}
