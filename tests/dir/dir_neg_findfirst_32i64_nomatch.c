/* TC
 * id:    dir_neg_findfirst_32i64_nomatch
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _findfirst32i64 _findnext32i64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _findfirst, _findnext (-1 + ENOENT when the spec cannot be matched; _findnext with an invalid handle returns -1)
 * note:  _findfirst32i64 gets its own test and runs the calls in a child process so that a missing/unimplemented export in a
 *        non-Microsoft runtime cannot hang or take down the parent. The child exits 0 if all expectations held, else a
 *        distinct code (10 = no match result wrong, 11 = errno wrong, 12 = findnext bad handle, 13 = match failed, 14 = close).
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

typedef intptr_t (__cdecl *ff_t)(const char *, void *);
typedef int (__cdecl *fn_t)(intptr_t, void *);
static union { long long al; char b[1024]; } fdb;
static const char *volatile v_nomatch = "tc_dir_neg_ff32i64_nomatch.zz";
static const char *volatile v_file = "tc_dir_neg_ff32i64_one.tmp";

static int child_main(void)
{
    ff_t ff = (ff_t)tc_crt_proc("_findfirst32i64"); fn_t fn = (fn_t)tc_crt_proc("_findnext32i64");
    intptr_t h; volatile intptr_t bad = -1; HANDLE fh;
    if (!ff || !fn) return 20;
    errno = 0; h = ff(v_nomatch, &fdb);
    if (h != -1) return 10;
    if (errno != ENOENT) return 11;
    if (fn(bad, &fdb) != -1) return 12;
    fh = CreateFileA(v_file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fh != INVALID_HANDLE_VALUE) CloseHandle(fh);
    h = ff(v_file, &fdb);
    if (h == -1) { DeleteFileA(v_file); return 13; }
    if (_findclose(h) != 0) { DeleteFileA(v_file); return 14; }
    DeleteFileA(v_file);
    return 0;
}

TC_CASE(nomatch_badhandle_close)
{
    DWORD code = 99; int ok = tc_spawn_self("ff32i64", &code, 8000);
    if (ok && code == 20) TC_SKIP("not exported");
    CHECK(ok);
    CHECK_EQ_U(code, 0);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(nomatch_badhandle_close);
    return tc_end();
}
