/* TC
 * id:    nfx_neg_assert_failure_aborts
 * area:  negative_fatal
 * kind:  negative
 * funcs: 
 * procs: _wassert _set_error_mode
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   C11 7.2.1.1; MS docs assert, _wassert: prints diagnostic then calls abort
 * note:  Error mode forced to _OUT_TO_STDERR (1) so no message box. Exit 3 relies on tc_begin clearing report fault.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

typedef void (__cdecl *wassert_t)(const wchar_t *, const wchar_t *, unsigned);
typedef int (__cdecl *sem_t)(int);
TC_CASE(assert_aborts)
{
    DWORD code = 0; int ran = tc_spawn_self("wa", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK(code != 78);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        wassert_t wa = (wassert_t)tc_crt_proc("_wassert");
        sem_t sem = (sem_t)tc_crt_proc("_set_error_mode");
        if (!wa || !sem) return 78;
        sem(1);
        wa(L"0", L"f.c", 1);
        return 77;
    }
    TC_RUN(assert_aborts);
    return tc_end();
}
