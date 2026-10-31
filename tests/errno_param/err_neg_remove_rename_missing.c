/* TC
 * id:    err_neg_remove_rename_missing
 * area:  errno_param
 * kind:  negative
 * funcs: remove rename
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs remove, rename
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#include <stdio.h>
#include <errno.h>

static const char *volatile v_a = "tc_err_neg_rr_missing_a" TC_S1(TC_CRT_VER) ".tmp";
static const char *volatile v_b = "tc_err_neg_rr_missing_b" TC_S1(TC_CRT_VER) ".tmp";

static void mk(const char *n) { HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); if (h != INVALID_HANDLE_VALUE) CloseHandle(h); }

TC_CASE(remove_missing) { int r; DeleteFileA(v_a); errno = 0; r = remove(v_a); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(rename_missing) { int r; DeleteFileA(v_a); DeleteFileA(v_b); errno = 0; r = rename(v_a, v_b); CHECK(r != 0); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(remove_twice)
{
    int r1, r2; mk(v_a);
    r1 = remove(v_a); errno = 0; r2 = remove(v_a);
    CHECK_EQ_I(r1, 0); CHECK_EQ_I(r2, -1); CHECK_EQ_I(errno, ENOENT);
}

int main(void)
{
    tc_begin();
    TC_RUN(remove_missing); TC_RUN(rename_missing); TC_RUN(remove_twice);
    DeleteFileA(v_a); DeleteFileA(v_b);
    return tc_end();
}
