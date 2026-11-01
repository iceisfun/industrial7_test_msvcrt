/* TC
 * id:    mem_neg_memccpy_null_params_probe
 * area:  memory
 * kind:  negative
 * funcs: _memccpy
 * crt:   >=140
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _memccpy (parameter validation not documented in detail)
 * note:  PROBES that _memccpy validates NULL dest / NULL src with count > 0: returns NULL, invokes the handler once,
 *        sets errno EINVAL. Believed for ucrtbase; older CRTs (msvcr80-120, whose _memccpy may be unvalidated
 *        and would crash) are deliberately not listed.
 *        Win10 22H2 19045 (observed): _memccpy(NULL, ...) and _memccpy(dst, NULL, ...) with count 4 are NOT validated (no handler call, no EINVAL): both die with
 *        0xC0000005. The two cases now run in child processes and assert that exit code (class C); case names kept.
 *        Case names (..._returns_null_handler_called_errno_EINVAL) are historical: on ucrtbase 19045 they now assert the observed access violation (0xC0000005) in a child, not a NULL return.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static char buf[8] = "abcdefg";
static void *volatile v_null = NULL;
static volatile size_t v_n = 4;

/* Windows (class C): _memccpy does not validate its pointers; a NULL dest/src is dereferenced and the process dies with an access violation. */
static void run(const char *n)
{
    DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(dest_null_returns_null_handler_called_errno_EINVAL) { run("dest"); }
TC_CASE(src_null_returns_null_handler_called_errno_EINVAL) { run("src"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_install();
        if (c[0] == 'd') _memccpy(v_null, buf, 'z', v_n); else _memccpy(buf, v_null, 'z', v_n);
        return 77;
    }
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup no handler API\n"); return 1; }
    TC_RUN(dest_null_returns_null_handler_called_errno_EINVAL); TC_RUN(src_null_returns_null_handler_called_errno_EINVAL);
    return tc_end();
}
