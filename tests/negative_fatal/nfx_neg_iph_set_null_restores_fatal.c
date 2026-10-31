/* TC
 * id:    nfx_neg_iph_set_null_restores_fatal
 * area:  negative_fatal
 * kind:  negative
 * funcs: _set_invalid_parameter_handler
 * procs: strcpy_s
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _set_invalid_parameter_handler; "Parameter validation"
 * note:  Setting a handler then NULL restores the default (terminating) behaviour. Exact status is in nfx_neg_iph_set_null_exact_ucrt.
 */
#include "tc.h"
TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);

#include <stdlib.h>
static void __cdecl h(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; ExitProcess(33); }

TC_CASE(null_restores_fatal)
{
    DWORD c = 0; CHECK(tc_spawn_self("setnull", &c, 20000));
    CHECK(c != 0); CHECK(c != 77); CHECK(c != 33);
}
TC_CASE(handler_runs)
{
    DWORD c = 0; CHECK(tc_spawn_self("setonly", &c, 20000)); CHECK_EQ_U(c, 33);
}
int main(void)
{
    const char *n; char d[4];
    tc_begin(); TC_LOAD_PROC(strcpy_s);
    if ((n = tc_child_name()) != NULL) {
        if (!tc_p_strcpy_s) return 78;
        _set_invalid_parameter_handler(h);
        if (n[3] == 'n') _set_invalid_parameter_handler(NULL);
        tc_p_strcpy_s(d, 0, "x");
        return 77;
    }
    TC_RUN(null_restores_fatal); TC_RUN(handler_runs);
    return tc_end();
}
