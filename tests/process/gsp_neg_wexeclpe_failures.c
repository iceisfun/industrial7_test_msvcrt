/* TC
 * id:    gsp_neg_wexeclpe_failures
 * area:  process
 * funcs: _wexeclpe
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _exec, _wexec: returns only on error, value -1; errno ENOENT / EINVAL
 * note:  NULL-cmdname validation is inferred (not spelled out in docs); ENOENT is documented. _wexeclpe never returns on success, so only failing calls are made. Nonexistent program -> -1/ENOENT with no handler
 *        call; NULL cmdname -> -1/EINVAL with one handler call (>=80). Empty-string cmdname runs in a child (exit 77 if it
 *        returned -1) so an unexpected success could not replace the test process; errno not asserted. NULL envp not asserted.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static const wchar_t *volatile g_prog = L"gsp_no_such.exe";
static const wchar_t *volatile g_nul = NULL;
static const wchar_t *volatile g_empty = L"";
static const wchar_t *g_envp[] = { L"GSP_X=1", NULL };

TC_CASE(nonexistent_enoent)
{
    intptr_t r;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _wexeclpe(g_prog, g_prog, (const wchar_t *)NULL, g_envp);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_cmdname)
{
    intptr_t r;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    r = _wexeclpe(g_nul, g_prog, (const wchar_t *)NULL, g_envp);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(empty_cmdname_child)
{
    DWORD code = 0; int ran = tc_spawn_self("empty", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        intptr_t r;
        tc_iph_install();
        r = _wexeclpe(g_empty, g_prog, (const wchar_t *)NULL, g_envp);
        return r == -1 ? 77 : 78;
    }
    TC_RUN(nonexistent_enoent); TC_RUN(empty_cmdname_child); TC_RUN(null_cmdname);
    return tc_end();
}
