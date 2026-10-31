/* TC
 * id:    nfx_neg_purecall_get_set_handler_null_default
 * area:  negative_fatal
 * kind:  negative
 * funcs: _purecall
 * procs: _set_purecall_handler _get_purecall_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _set_purecall_handler (returns previous handler), _get_purecall_handler, _purecall (default handler terminates)
 * note:  Probes that passing NULL to _set_purecall_handler is accepted, is reported back by the getter as NULL, and restores the
 *        terminating default (not separately documented).
 *        Child installs h, then _set_purecall_handler(NULL) must return h and the getter must then return NULL; _purecall then
 *        terminates abnormally (not 0, not 77) and h must not have run (marker file absent).
 */
#include "tc.h"

typedef void (__cdecl *pc_t)(void);
extern void __cdecl _purecall(void);
#define MK "tc_nfx_neg_purecall_get_set_handler_null_default_m.tmp"
static void __cdecl h(void)
{
    HANDLE f = CreateFileA(MK, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f != INVALID_HANDLE_VALUE) CloseHandle(f);
    ExitProcess(42);
}
TC_CASE(null_restores_default)
{
    DWORD code = 0, at; int ran;
    DeleteFileA(MK);
    ran = tc_spawn_self("pc", &code, 20000);
    at = GetFileAttributesA(MK);
    DeleteFileA(MK);
    CHECK(ran);
    CHECK(code != 0); CHECK(code != 77); CHECK(code != 42);
    CHECK(code != 71 && code != 72 && code != 73 && code != 78); /* child self-check failures, not a purecall termination */
    CHECK(at == INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        pc_t (__cdecl *set)(pc_t) = (pc_t (__cdecl *)(pc_t))tc_crt_proc("_set_purecall_handler");
        pc_t (__cdecl *get)(void) = (pc_t (__cdecl *)(void))tc_crt_proc("_get_purecall_handler");
        if (!set || !get) return 78;
        set(h);
        if (get() != h) return 71;
        if (set(NULL) != h) return 72;
        if (get() != NULL) return 73;
        _purecall();
        return 77;
    }
    TC_RUN(null_restores_default);
    return tc_end();
}
