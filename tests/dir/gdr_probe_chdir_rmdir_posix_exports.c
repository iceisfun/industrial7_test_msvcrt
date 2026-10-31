/* TC
 * id:    gdr_probe_chdir_rmdir_posix_exports
 * area:  dir
 * funcs:
 * procs: chdir rmdir
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs chdir/rmdir (deprecated POSIX names); UCRT moved them to the static oldnames lib
 * note:  PROBE: ucrtbase must NOT export un-underscored chdir/rmdir (asserted NULL). Whether msvcrt.dll and msvcr80-120 export
 *        them is only OBSERVED (printed as a TC NOTE line, no assertion): they normally come from oldnames thunks, not the DLL.
 *        If present anywhere they are exercised.
 */
#include "tc.h"

typedef int (__cdecl *fn1_t)(const char *);
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define D "tc_gdr_posix_exports" TC_S(TC_CRT_VER) ".tmp"

TC_CASE(presence)
{
    FARPROC c = tc_crt_proc("chdir"), r = tc_crt_proc("rmdir");
    tc_write("TC NOTE " TC_TEST_ID " chdir="); tc_write(c ? "present" : "absent");
    tc_write(" rmdir="); tc_write(r ? "present" : "absent"); tc_write("\n");
#ifdef TC_UCRT
    CHECK(c == NULL); CHECK(r == NULL);
#endif
}
TC_CASE(works_if_present)
{
    fn1_t c = (fn1_t)tc_crt_proc("chdir"), r = (fn1_t)tc_crt_proc("rmdir");
    char saved[MAX_PATH], now[MAX_PATH];
    if (!c || !r) TC_SKIP("not exported");
    RemoveDirectoryA(D);
    CreateDirectoryA(D, NULL);
    GetCurrentDirectoryA(MAX_PATH, saved);
    CHECK_EQ_I(c(D), 0);
    GetCurrentDirectoryA(MAX_PATH, now);
    CHECK(lstrlenA(now) > lstrlenA(saved));
    SetCurrentDirectoryA(saved);
    CHECK_EQ_I(r(D), 0);
    CHECK(GetFileAttributesA(D) == INVALID_FILE_ATTRIBUTES);
    RemoveDirectoryA(D);
}
int main(void) { tc_begin(); TC_RUN(presence); TC_RUN(works_if_present); return tc_end(); }
