/* TC
 * id:    err_neg_rename_onto_existing_probe
 * area:  errno_param
 * kind:  negative
 * funcs: rename
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs rename (EACCES if newname exists), Windows error mapping ERROR_ALREADY_EXISTS->EEXIST
 * note:  PROBE: rename onto an existing file must fail (nonzero, documented); the errno is probed: MS docs say EACCES but the Win32 error ERROR_ALREADY_EXISTS maps to EEXIST. Test asserts EEXIST.
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#include <stdio.h>
#include <errno.h>

static const char *volatile v_a = "tc_err_neg_ren_exist_a" TC_S1(TC_CRT_VER) ".tmp";
static const char *volatile v_b = "tc_err_neg_ren_exist_b" TC_S1(TC_CRT_VER) ".tmp";

static void mk(const char *n) { HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); if (h != INVALID_HANDLE_VALUE) CloseHandle(h); }

TC_CASE(fails) { int r; mk(v_a); mk(v_b); errno = 0; r = rename(v_a, v_b); CHECK(r != 0); CHECK(GetFileAttributesA(v_a) != INVALID_FILE_ATTRIBUTES); }
TC_CASE(errno_eexist) { int r; mk(v_a); mk(v_b); errno = 0; r = rename(v_a, v_b); CHECK(r != 0); CHECK_EQ_I(errno, EEXIST); }

int main(void)
{
    tc_begin();
    TC_RUN(fails); TC_RUN(errno_eexist);
    DeleteFileA(v_a); DeleteFileA(v_b);
    return tc_end();
}
