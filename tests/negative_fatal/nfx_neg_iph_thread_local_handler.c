/* TC
 * id:    nfx_neg_iph_thread_local_handler
 * area:  negative_fatal
 * kind:  negative
 * funcs: 
 * procs: _set_thread_local_invalid_parameter_handler _set_invalid_parameter_handler strcpy_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_thread_local_invalid_parameter_handler
 * note:  Thread-local handler takes precedence over the global one; NULL falls back to global. The third case (handler set only on another thread, main thread uses default) is a probe-level inference but only asserts termination, kept as its own case.
 */
#include "tc.h"
typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(iph_t, _set_thread_local_invalid_parameter_handler, (iph_t));
TC_DECL_PROC(iph_t, _set_invalid_parameter_handler, (iph_t));
static void __cdecl g40(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; ExitProcess(40); }
static void __cdecl t41(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; ExitProcess(41); }
static DWORD WINAPI thr(LPVOID p) { (void)p; tc_p__set_thread_local_invalid_parameter_handler(t41); return 0; }

TC_CASE(tls_over_global) { DWORD c = 0; CHECK(tc_spawn_self("both", &c, 20000)); CHECK_EQ_U(c, 41); }
TC_CASE(null_falls_back) { DWORD c = 0; CHECK(tc_spawn_self("fallback", &c, 20000)); CHECK_EQ_U(c, 40); }
TC_CASE(other_thread_not_used)
{
    DWORD c = 0; CHECK(tc_spawn_self("otherthread", &c, 20000));
    CHECK(c != 0); CHECK(c != 77); CHECK(c != 41);
}
int main(void)
{
    const char *n; char d[4];
    tc_begin();
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(_set_thread_local_invalid_parameter_handler); TC_LOAD_PROC(_set_invalid_parameter_handler);
    if ((n = tc_child_name()) != NULL) {
        if (!tc_p_strcpy_s || !tc_p__set_thread_local_invalid_parameter_handler || !tc_p__set_invalid_parameter_handler) return 78;
        if (n[0] == 'o') {
            HANDLE t = CreateThread(NULL, 0, thr, NULL, 0, NULL);
            if (!t) return 79;
            WaitForSingleObject(t, 10000); CloseHandle(t);
        } else {
            tc_p__set_invalid_parameter_handler(g40);
            tc_p__set_thread_local_invalid_parameter_handler(t41);
            if (n[0] == 'f') tc_p__set_thread_local_invalid_parameter_handler(NULL);
        }
        tc_p_strcpy_s(d, 0, "x");
        return 77;
    }
    CHECK(tc_p__set_thread_local_invalid_parameter_handler != NULL);
    TC_RUN(tls_over_global); TC_RUN(null_falls_back); TC_RUN(other_thread_not_used);
    return tc_end();
}
