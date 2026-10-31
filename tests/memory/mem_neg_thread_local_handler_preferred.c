/* TC
 * id:    mem_neg_thread_local_handler_preferred
 * area:  memory
 * funcs:
 * procs: memcpy_s memmove_s _memicmp _set_invalid_parameter_handler _set_thread_local_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_thread_local_invalid_parameter_handler (thread handler overrides the process one for that thread); memcpy_s, memmove_s, _memicmp error tables
 * note:  Uses threads deliberately: precedence between the process-wide and thread-local handler is the subject. Short-lived worker thread.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, memmove_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, _memicmp, (const void *, const void *, size_t));
typedef void (__cdecl *ih_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static ih_t (__cdecl *p_set)(ih_t);
static ih_t (__cdecl *p_setl)(ih_t);
static volatile LONG ca, cb;
static void __cdecl ha(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; InterlockedIncrement(&ca); }
static void __cdecl hb(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; InterlockedIncrement(&cb); }
static volatile size_t v_n = 4;
static volatile int t_ret = -1;

static DWORD WINAPI worker(LPVOID p)
{
    (void)p;
    t_ret = tc_p_memcpy_s(NULL, 8, "abcd", v_n);
    return 0;
}
TC_CASE(thread_local_wins)
{
    int r;
    if (!tc_p_memcpy_s || !tc_p_memmove_s || !tc_p__memicmp || !p_set || !p_setl) TC_SKIP("missing");
    ca = cb = 0; p_set(ha); p_setl(hb);
    r = tc_p_memcpy_s(NULL, 8, "abcd", v_n);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(cb, 1); CHECK_EQ_I(ca, 0);
    r = tc_p_memmove_s(NULL, 8, "abcd", v_n);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(cb, 2); CHECK_EQ_I(ca, 0);
    (void)tc_p__memicmp(NULL, "abcd", v_n);
    CHECK_EQ_I(cb, 3); CHECK_EQ_I(ca, 0);
}
TC_CASE(other_thread_uses_process_handler)
{
    HANDLE t; DWORD w;
    if (!tc_p_memcpy_s || !p_set || !p_setl) TC_SKIP("missing");
    ca = cb = 0; t_ret = -1; p_set(ha); p_setl(hb);
    t = CreateThread(NULL, 0, worker, NULL, 0, NULL);
    CHECK(t != NULL);
    if (!t) return;
    w = WaitForSingleObject(t, 10000); CloseHandle(t);
    CHECK_EQ_I(w, WAIT_OBJECT_0);
    CHECK_EQ_I(t_ret, EINVAL); CHECK_EQ_I(ca, 1); CHECK_EQ_I(cb, 0);
}
TC_CASE(reset_falls_back_to_process)
{
    int r;
    if (!tc_p_memcpy_s || !p_set || !p_setl) TC_SKIP("missing");
    ca = cb = 0; p_set(ha); p_setl(hb); p_setl(NULL);
    r = tc_p_memcpy_s(NULL, 8, "abcd", v_n);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(ca, 1); CHECK_EQ_I(cb, 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(memcpy_s); TC_LOAD_PROC(memmove_s); TC_LOAD_PROC(_memicmp);
    p_set = (ih_t (__cdecl *)(ih_t))tc_crt_proc("_set_invalid_parameter_handler");
    p_setl = (ih_t (__cdecl *)(ih_t))tc_crt_proc("_set_thread_local_invalid_parameter_handler");
    TC_RUN(thread_local_wins); TC_RUN(other_thread_uses_process_handler); TC_RUN(reset_falls_back_to_process);
    return tc_end();
}
