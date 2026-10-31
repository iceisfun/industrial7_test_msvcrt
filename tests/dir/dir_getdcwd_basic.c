/* TC
 * id:    dir_getdcwd_basic
 * area:  dir
 * funcs: _getdcwd _getdrive _wgetdcwd _getcwd
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _getdcwd: drive 0 = default drive, 1 = A, 2 = B
 * note:  
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_zero;

static void cur(char *b) { DWORD n = GetCurrentDirectoryA(MAX_PATH, b); if (!n || n >= MAX_PATH) b[0] = 0; }

TC_CASE(drive0_equals_getcwd)
{
    char a[MAX_PATH], b[MAX_PATH];
    CHECK(_getdcwd(0 + v_zero, a, MAX_PATH) != NULL);
    CHECK(_getcwd(b, MAX_PATH) != NULL);
    CHECK_STR(a, b);
}
TC_CASE(current_drive)
{
    char a[MAX_PATH], ref[MAX_PATH]; int d = _getdrive(); cur(ref);
    CHECK(d >= 1 && d <= 26);
    CHECK(_getdcwd(d, a, MAX_PATH) != NULL);
    CHECK_STR(a, ref);
}
TC_CASE(drive_prefix)
{
    char a[MAX_PATH]; int d = _getdrive();
    CHECK(_getdcwd(d, a, MAX_PATH) != NULL);
    CHECK_EQ_I(a[0] & ~0x20, 'A' + d - 1);
    CHECK_EQ_I(a[1], ':');
    CHECK_EQ_I(a[2], '\\');
}
TC_CASE(null_alloc)
{
    char ref[MAX_PATH]; char *p; cur(ref);
    p = _getdcwd(0, NULL, 0 + v_zero);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, ref); free(p); }
}
TC_CASE(wide_null_alloc)
{
    wchar_t ref[MAX_PATH]; wchar_t *p; DWORD n = GetCurrentDirectoryW(MAX_PATH, ref);
    CHECK(n > 0 && n < MAX_PATH);
    p = _wgetdcwd(0, NULL, 0 + v_zero);
    CHECK(p != NULL);
    if (p) { CHECK_WSTR(p, ref); free(p); }
}
int main(void)
{
    tc_begin();
    TC_RUN(drive0_equals_getcwd); TC_RUN(current_drive); TC_RUN(drive_prefix); TC_RUN(null_alloc); TC_RUN(wide_null_alloc);
    return tc_end();
}
