/* TC
 * id:    dir_neg_findnext_exhausted
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _findfirst64i32 _findnext64i32
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _findnext: returns -1 and errno ENOENT when no more matching files; _findclose returns 0 on success
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define D "tc_dir_neg_findnext_exhausted_d" TC_S(TC_CRT_VER) ".tmp"
typedef intptr_t (__cdecl *ff_t)(const char *, struct _finddata64i32_t *);
typedef int (__cdecl *fn_t)(intptr_t, struct _finddata64i32_t *);
static const char *volatile v_spec;

static void mkfile(const char *p, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char c = 0x41; DWORD w; int i;
    if (h == INVALID_HANDLE_VALUE) return;
    for (i = 0; i < n; i++) WriteFile(h, &c, 1, &w, NULL);
    CloseHandle(h);
}
static void cleanup(void) { DeleteFileA(D "\\only.one"); RemoveDirectoryA(D); }

TC_CASE(exhausted)
{
    ff_t ff = (ff_t)tc_crt_proc("_findfirst64i32"); fn_t fn = (fn_t)tc_crt_proc("_findnext64i32");
    struct _finddata64i32_t fd; intptr_t h;
    if (!ff || !fn) TC_SKIP("not exported");
    CreateDirectoryA(D, NULL); mkfile(D "\\only.one", 4);
    v_spec = D "\\*.one"; h = ff(v_spec, &fd);
    CHECK(h != -1);
    if (h == -1) return;
    CHECK_STR(fd.name, "only.one");
    errno = 0; CHECK_EQ_I(fn(h, &fd), -1); CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(_findclose(h), 0);
}
int main(void) { tc_begin(); cleanup(); TC_RUN(exhausted); cleanup(); return tc_end(); }
