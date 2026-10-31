/* TC
 * id:    prc_fmode_not_inherited_probe
 * area:  process
 * funcs: _spawnl
 * procs: __p__fmode _set_fmode
 * crt:   >=80
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _fmode, _spawn
 * note:  Probes that _fmode is process-local: parent sets _O_BINARY, a spawned self reports *__p__fmode()
 *        as its exit code: 0 if the value is not _O_BINARY
 *        (default 0 or _O_TEXT, see prc_fmode_default_probe), 50 if _O_BINARY leaked from the parent.
 */
#include "tc.h"
#include <process.h>
#include <fcntl.h>

TC_DECL_PROC(int *, __p__fmode, (void));
TC_DECL_PROC(int, _set_fmode, (int));

TC_CASE(child_default)
{
    char exe[MAX_PATH]; intptr_t r; int orig = *tc_p___p__fmode();
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    CHECK_EQ_I(tc_p__set_fmode(_O_BINARY), 0);
    r = _spawnl(_P_WAIT, exe, exe, "--tc-child=fm", NULL);
    *tc_p___p__fmode() = orig;
    CHECK_EQ_I(r, 0);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(__p__fmode); TC_LOAD_PROC(_set_fmode);
    if (!tc_p___p__fmode || !tc_p__set_fmode) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    if (tc_child_name() != NULL) return *tc_p___p__fmode() != _O_BINARY ? 0 : 50;
    TC_RUN(child_default);
    return tc_end();
}
