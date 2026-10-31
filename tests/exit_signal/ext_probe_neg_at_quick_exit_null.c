/* TC
 * id:    ext_probe_neg_at_quick_exit_null
 * area:  exit_signal
 * funcs: _crt_at_quick_exit quick_exit
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.4.3 / MS docs quick_exit, at_quick_exit
 * note:  PROBES (1) _crt_at_quick_exit(NULL) returns 0 and (2) a later quick_exit(5) does not crash, runs the real handler
 *        (marker file written) and exits with code 5. Case 'null_return' is separate from 'survives'.
 */
#include "tc.h"
#include <stdlib.h>

#define MARK "tc_ext_probe_at_quick_exit_null.tmp"
extern int __cdecl _crt_at_quick_exit(void (__cdecl *)(void));
static void __cdecl real(void)
{
    HANDLE h = CreateFileA(MARK, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}
TC_CASE(null_return)
{ DWORD c = 0; CHECK(tc_spawn_self("ret", &c, 20000)); CHECK_EQ_U(c, 10); }
TC_CASE(survives)
{
    DWORD c = 0; DeleteFileA(MARK);
    CHECK(tc_spawn_self("qe", &c, 20000));
    CHECK_EQ_U(c, 5);
    CHECK(GetFileAttributesA(MARK) != INVALID_FILE_ATTRIBUTES);
    DeleteFileA(MARK);
}
int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        int r = _crt_at_quick_exit((void (__cdecl *)(void))(volatile void *)NULL);
        if (n[0] == 'r') ExitProcess(r == 0 ? 10 : 11);
        if (_crt_at_quick_exit(real) != 0) return 12;
        quick_exit(5);
        return 77;
    }
    TC_RUN(null_return); TC_RUN(survives);
    return tc_end();
}
