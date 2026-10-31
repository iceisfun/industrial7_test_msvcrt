/* TC
 * id:    nfx_neg_terminate_get_handler_default_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs:
 * procs: ?set_terminate@@YAP6AXXZP6AXXZ@Z ?terminate@@YAXXZ
 * crt:   win7
 * tier:  p2
 * conf:  probe
 * ref:   MS docs set_terminate, terminate (default terminate calls abort); abort (exit code 3, raises SIGABRT)
 * note:  Probes on the old CRTs (C++ names, resolved by decorated name): the initial terminate handler is NULL (set_terminate returns
 *        NULL), and default terminate() raises SIGABRT (user handler seen, marker file written, handler returns) and exits with code 3.
 */
#include "tc.h"
#include <signal.h>

typedef void (__cdecl *th_t)(void);
#define MK "tc_nfx_neg_terminate_get_handler_default_probe_m.tmp"
static void __cdecl ab(int s)
{
    HANDLE f = CreateFileA(MK, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD n;
    (void)s;
    if (f != INVALID_HANDLE_VALUE) { WriteFile(f, "S", 1, &n, NULL); CloseHandle(f); }
}
static void __cdecl dummy(void) { }

TC_CASE(initial_handler_null)
{
    DWORD c = 99; CHECK(tc_spawn_self("init", &c, 20000)); CHECK_EQ_U(c, 0);
}
TC_CASE(default_terminate_exit3_sigabrt_seen)
{
    DWORD c = 0, at; int ran;
    DeleteFileA(MK);
    ran = tc_spawn_self("dflt", &c, 20000);
    at = GetFileAttributesA(MK);
    DeleteFileA(MK);
    CHECK(ran); CHECK_EQ_U(c, 3);
    CHECK(at != INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    const char *n; tc_begin();
    if ((n = tc_child_name()) != NULL) {
        th_t (__cdecl *set)(th_t) = (th_t (__cdecl *)(th_t))tc_crt_proc("?set_terminate@@YAP6AXXZP6AXXZ@Z");
        void (__cdecl *term)(void) = (void (__cdecl *)(void))tc_crt_proc("?terminate@@YAXXZ");
        if (!set || !term) return 78;
        if (n[0] == 'i') return set(dummy) == NULL ? 0 : 1;
        signal(SIGABRT, ab);
        term();
        return 77;
    }
    TC_RUN(initial_handler_null); TC_RUN(default_terminate_exit3_sigabrt_seen);
    return tc_end();
}
