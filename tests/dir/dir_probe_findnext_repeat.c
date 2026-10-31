/* TC
 * id:    dir_probe_findnext_repeat
 * area:  dir
 * kind:  positive
 * funcs: _findclose
 * procs: _findfirst64i32 _findnext64i32
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _findnext
 * note:  Probes that calling _findnext AGAIN after it already reported exhaustion (-1/ENOENT) keeps returning -1 with
 *        ENOENT (FindNextFile on an exhausted handle keeps failing with ERROR_NO_MORE_FILES). Not documented explicitly.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define D "tc_dir_probe_findnext_repeat_d" TC_S(TC_CRT_VER) ".tmp"
typedef intptr_t (__cdecl *ff_t)(const char *, struct _finddata64i32_t *);
typedef int (__cdecl *fn_t)(intptr_t, struct _finddata64i32_t *);
static const char *volatile v_spec;

static void cleanup(void) { DeleteFileA(D "\\only.one"); RemoveDirectoryA(D); }

TC_CASE(repeat)
{
    ff_t ff = (ff_t)tc_crt_proc("_findfirst64i32"); fn_t fn = (fn_t)tc_crt_proc("_findnext64i32");
    struct _finddata64i32_t fd; intptr_t h; HANDLE f;
    if (!ff || !fn) TC_SKIP("not exported");
    CreateDirectoryA(D, NULL);
    f = CreateFileA(D "\\only.one", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f != INVALID_HANDLE_VALUE) CloseHandle(f);
    v_spec = D "\\*.one"; h = ff(v_spec, &fd);
    CHECK(h != -1);
    if (h == -1) return;
    CHECK_EQ_I(fn(h, &fd), -1);
    errno = 0; CHECK_EQ_I(fn(h, &fd), -1); CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(_findclose(h), 0);
}
int main(void) { tc_begin(); cleanup(); TC_RUN(repeat); cleanup(); return tc_end(); }
