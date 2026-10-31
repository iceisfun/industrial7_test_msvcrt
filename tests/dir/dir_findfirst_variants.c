/* TC
 * id:    dir_findfirst_variants
 * area:  dir
 * funcs: _findfirst32 _findnext32 _findfirst32i64 _findnext32i64 _findfirst64 _findnext64 _findclose
 * procs: _findfirst64i32 _findnext64i32
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _findfirst, _findnext (struct variants _finddata32_t, _finddata32i64_t, _finddata64i32_t, __finddata64_t)
 * note:  _findfirst64i32 is an inline wrapper in mingw headers, so it is resolved at run time. time_write is only sanity checked.
 */
#include "tc.h"
#include <io.h>
#include <time.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define D "tc_dir_findfirst_variants_d" TC_S(TC_CRT_VER) ".tmp"
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
static void cleanup(void) { DeleteFileA(D "\\ten.bin"); DeleteFileA(D "\\zero.bin"); RemoveDirectoryA(D); }
#define SANE_2000 946684800LL   /* 2000-01-01 */

TC_CASE(sizeof_fields)
{
    struct _finddata32_t a; struct _finddata32i64_t b; struct _finddata64i32_t c; struct __finddata64_t d;
    CHECK_EQ_U(sizeof a.time_write, 4); CHECK_EQ_U(sizeof a.size, 4);
    CHECK_EQ_U(sizeof b.time_write, 4); CHECK_EQ_U(sizeof b.size, 8);
    CHECK_EQ_U(sizeof c.time_write, 8); CHECK_EQ_U(sizeof c.size, 4);
    CHECK_EQ_U(sizeof d.time_write, 8); CHECK_EQ_U(sizeof d.size, 8);
}
TC_CASE(v32)
{
    struct _finddata32_t fd; intptr_t h; CreateDirectoryA(D, NULL); mkfile(D "\\ten.bin", 10); mkfile(D "\\zero.bin", 0);
    v_spec = D "\\ten.bin"; h = _findfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_STR(fd.name, "ten.bin"); CHECK_EQ_U(fd.size, 10); CHECK(fd.time_write > SANE_2000);
        CHECK_EQ_I(_findnext32(h, &fd), -1); CHECK_EQ_I(_findclose(h), 0); }
    v_spec = D "\\zero.bin"; h = _findfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_EQ_U(fd.size, 0); _findclose(h); }
}
TC_CASE(v32i64)
{
    struct _finddata32i64_t fd; intptr_t h;
    v_spec = D "\\ten.bin"; h = _findfirst32i64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_STR(fd.name, "ten.bin"); CHECK_EQ_U(fd.size, 10); CHECK(fd.time_write > SANE_2000);
        CHECK_EQ_I(_findnext32i64(h, &fd), -1); CHECK_EQ_I(_findclose(h), 0); }
}
TC_CASE(v64i32)
{
    struct _finddata64i32_t fd; intptr_t h;
    if (!p_ff || !p_fn) TC_SKIP("not exported");
    v_spec = D "\\ten.bin"; h = p_ff(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_STR(fd.name, "ten.bin"); CHECK_EQ_U(fd.size, 10); CHECK(fd.time_write > SANE_2000);
        CHECK_EQ_I(p_fn(h, &fd), -1); CHECK_EQ_I(_findclose(h), 0); }
}
TC_CASE(v64)
{
    struct __finddata64_t fd; intptr_t h;
    v_spec = D "\\ten.bin"; h = _findfirst64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_STR(fd.name, "ten.bin"); CHECK_EQ_U(fd.size, 10); CHECK(fd.time_write > SANE_2000);
        CHECK_EQ_I(_findnext64(h, &fd), -1); CHECK_EQ_I(_findclose(h), 0); }
    v_spec = D "\\zero.bin"; h = _findfirst64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_EQ_U(fd.size, 0); _findclose(h); }
}
int main(void)
{
    tc_begin(); cleanup();
    p_ff = (ff_t)tc_crt_proc("_findfirst64i32"); p_fn = (fn_t)tc_crt_proc("_findnext64i32");
    TC_RUN(sizeof_fields); TC_RUN(v32); TC_RUN(v32i64); TC_RUN(v64i32); TC_RUN(v64);
    cleanup();
    return tc_end();
}
