/* TC
 * id:    mem_neg_fatal_default_handler_wmemcpy_s
 * area:  memory
 * kind:  negative
 * funcs:
 * procs: wmemcpy_s wmemmove_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation": default invalid-parameter handler terminates the process
 * note:  Fatal paths run in child processes; asserts abnormal termination (not 0/77/78/79), not the exact status
 *        (ucrtbase is expected to give 0xC0000409, older CRTs may differ). 78 = proc missing, 79 = control failed.
 *        _memicmp / _memicmp_l NULL cases live in mem_neg_fatal_default_handler_memicmp_l_probe (not documented).
 */
#include "tc.h"
#include <string.h>

typedef int (__cdecl *wcp_t)(wchar_t *, size_t, const wchar_t *, size_t);
static volatile wchar_t ws[4] = { 1, 2, 3, 0 }, wd[4];

static int child_main(const char *n)
{
    wcp_t cpy = (wcp_t)tc_crt_proc("wmemcpy_s"), mov = (wcp_t)tc_crt_proc("wmemmove_s");
    if (!cpy || !mov) return 78;
    if (tc_strequ(n, "cpy_null_dest")) cpy(NULL, 4, (const wchar_t *)ws, 2);
    else if (tc_strequ(n, "mov_range")) mov((wchar_t *)wd, 2, (const wchar_t *)ws, 4);
    else if (tc_strequ(n, "ok")) {
        if (cpy((wchar_t *)wd, 4, (const wchar_t *)ws, 2) != 0) return 79;
        if (mov((wchar_t *)wd, 4, (const wchar_t *)ws, 2) != 0) return 79;
    }
    return 77;
}
static void run(const char *n, int fatal)
{
    DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran);
    if (fatal) { CHECK(code != 0); CHECK(code != 77); CHECK(code != 78); CHECK(code != 79); }
    else CHECK_EQ_U(code, 77);
}
TC_CASE(child_wmemcpy_s_null_dest) { run("cpy_null_dest", 1); }
TC_CASE(child_wmemmove_s_range) { run("mov_range", 1); }
TC_CASE(child_valid_reaches_77) { run("ok", 0); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_wmemcpy_s_null_dest); TC_RUN(child_wmemmove_s_range); TC_RUN(child_valid_reaches_77);
    return tc_end();
}
