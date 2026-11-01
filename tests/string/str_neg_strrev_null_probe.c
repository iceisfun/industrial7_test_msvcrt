/* TC
 * id:    str_neg_strrev_null_probe
 * area:  string
 * funcs: _strrev
 * procs: 
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _strrev (lists no error return)
 * note:  PROBE: _strrev(NULL) returns NULL, errno EINVAL, invalid parameter handler called once. Undocumented; UCRT is believed to validate.
 *        Win10 22H2 19045 (observed): _strrev(NULL) crashes with 0xC0000005 (no validation, no handler call); the case now runs in a child process and asserts that
 *        exit code (class C).
 *        crt narrowed to ucrtbase: the crash / fast-fail behaviour was observed only on ucrtbase 19045; older CRTs are [unverified] (original expectation not asserted there).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile p_rev)(char *) = _strrev;
/* Windows (class C): _strrev(NULL) is not validated; it dereferences NULL and the process dies with an access violation, so it runs in a child. */
TC_CASE(null_probe)
{
    DWORD code = 0; int ran = tc_spawn_self("strrev_null", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) { tc_iph_install(); p_rev(NULL); return 77; }
    TC_RUN(null_probe);
    return tc_end();
}
