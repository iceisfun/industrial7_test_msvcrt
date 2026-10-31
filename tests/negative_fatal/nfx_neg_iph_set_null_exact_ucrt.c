/* TC
 * id:    nfx_neg_iph_set_null_exact_ucrt
 * area:  negative_fatal
 * kind:  negative
 * funcs: _set_invalid_parameter_handler
 * procs: strcpy_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation"
 * note:  Probes exact status 0xC0000409 after set(h); set(NULL); bad strcpy_s.
 */
#include "tc.h"
TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);

#include <stdlib.h>
static void __cdecl h(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; ExitProcess(33); }
TC_CASE(exact) { DWORD c = 0; CHECK(tc_spawn_self("setnull", &c, 20000)); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }
int main(void)
{
    char d[4];
    tc_begin(); TC_LOAD_PROC(strcpy_s);
    if (tc_child_name()) {
        if (!tc_p_strcpy_s) return 78;
        _set_invalid_parameter_handler(h); _set_invalid_parameter_handler(NULL);
        tc_p_strcpy_s(d, 0, "x");
        return 77;
    }
    TC_RUN(exact);
    return tc_end();
}
