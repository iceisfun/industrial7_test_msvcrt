/* TC
 * id:    dir_neg_findfirst_nomatch
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _findfirst64i32
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _findfirst: returns -1 and errno ENOENT if the specification could not be matched
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define D "tc_dir_neg_findfirst_nomatch_d" TC_S(TC_CRT_VER) ".tmp"
typedef intptr_t (__cdecl *ff_t)(const char *, struct _finddata64i32_t *);
static ff_t p_ff;
static const char *volatile v_spec;

static void mkfile(const char *p, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char c = 0x41; DWORD w; int i;
    if (h == INVALID_HANDLE_VALUE) return;
    for (i = 0; i < n; i++) WriteFile(h, &c, 1, &w, NULL);
    CloseHandle(h);
}
static void cleanup(void) { DeleteFileA(D "\\a.txt"); RemoveDirectoryA(D); }

TC_CASE(no_match_pattern)
{
    struct _finddata64i32_t fd; intptr_t h;
    if (!p_ff) TC_SKIP("not exported");
    CreateDirectoryA(D, NULL); mkfile(D "\\a.txt", 1);
    v_spec = D "\\*.zzz"; errno = 0; h = p_ff(v_spec, &fd);
    CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, ENOENT);
    if (h != -1) _findclose(h);
}
TC_CASE(missing_dir)
{
    struct _finddata64i32_t fd; intptr_t h;
    if (!p_ff) TC_SKIP("not exported");
    v_spec = "tc_dir_neg_findfirst_nomatch_nodir_tmp\\*.txt"; errno = 0; h = p_ff(v_spec, &fd);
    CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, ENOENT);
    if (h != -1) _findclose(h);
}
TC_CASE(no_leak_loop)
{
    struct _finddata64i32_t fd; int i, bad = 0;
    if (!p_ff) TC_SKIP("not exported");
    for (i = 0; i < 2000; i++) {
        intptr_t h; v_spec = D "\\*.zzz"; h = p_ff(v_spec, &fd);
        if (h != -1) { bad++; _findclose(h); }
    }
    CHECK_EQ_I(bad, 0);
}
int main(void)
{
    tc_begin(); cleanup();
    p_ff = (ff_t)tc_crt_proc("_findfirst64i32");
    TC_RUN(no_match_pattern); TC_RUN(missing_dir); TC_RUN(no_leak_loop);
    cleanup();
    return tc_end();
}
