/* TC
 * id:    gsm_neg_strlwr_null_fatal_child
 * area:  string
 * kind:  negative
 * funcs:
 * procs: _strlwr_s _strupr_s _strset_s _wcsset_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation": the default invalid-parameter handler terminates the process
 * note:  Termination is documented; the exact status (fast-fail 0xC0000409) is the probe, as in err_neg_default_handler_fatal.
 *        One child per function with the default handler; control children with a handler installed see EINVAL and exit 77.
 */
#include "tc.h"
TC_DECL_PROC(int, _strlwr_s, (char *, size_t));
TC_DECL_PROC(int, _strupr_s, (char *, size_t));
TC_DECL_PROC(int, _strset_s, (char *, size_t, int));
TC_DECL_PROC(int, _wcsset_s, (wchar_t *, size_t, wchar_t));

static int trigger(int k)
{
    char *volatile np = NULL; char nt[4]; wchar_t w[4] = { L'a', 0, 0, 0 };
    nt[0] = 'a'; nt[1] = 'b'; nt[2] = 'c'; nt[3] = 'd';
    switch (k) {
    case 0: return tc_p__strlwr_s(np, 10);
    case 1: return tc_p__strupr_s(nt, 4);
    case 2: return tc_p__strset_s(np, 8, 'x');
    default: return tc_p__wcsset_s(w, 0, L'x');
    }
}
static void run_all(int handler)
{
    static const char *const nm[] = { "lwr", "upr", "set", "wset" };
    int i;
    for (i = 0; i < 4; i++) {
        char name[8]; DWORD code = 0; int ran, ok;
        name[0] = handler ? 'i' : 'd'; name[1] = (char)('0' + i); name[2] = 0;
        ran = tc_spawn_self(name, &code, 20000);
        ok = ran && (handler ? code == 77 : (code != 0 && code != 77));
        if (!ok) tc_fail_expr(__FILE__, __LINE__, nm[i]);
        if (!handler && ran) CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
    }
}
TC_CASE(missing_procs) { CHECK(0 && "secure procs not exported"); }
TC_CASE(default_terminates) { run_all(0); }
TC_CASE(handler_returns_einval) { run_all(1); }
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_strlwr_s); TC_LOAD_PROC(_strupr_s); TC_LOAD_PROC(_strset_s); TC_LOAD_PROC(_wcsset_s);
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'i') { tc_iph_install(); return trigger(c[1] - '0') == 22 ? 77 : 78; }
        trigger(c[1] - '0'); return 77;
    }
    if (!(tc_p__strlwr_s && tc_p__strupr_s && tc_p__strset_s && tc_p__wcsset_s)) TC_RUN(missing_procs);
    else
    { TC_RUN(default_terminates); TC_RUN(handler_returns_einval); }
    return tc_end();
}
