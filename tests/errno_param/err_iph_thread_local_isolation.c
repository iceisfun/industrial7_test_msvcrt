/* TC
 * id:    err_iph_thread_local_isolation
 * area:  errno_param
 * funcs: strcpy_s _set_invalid_parameter_handler
 * procs: _set_thread_local_invalid_parameter_handler _get_thread_local_invalid_parameter_handler
 * crt:   >=140
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_thread_local_invalid_parameter_handler (per-thread)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

typedef void (__cdecl *ih_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
typedef ih_t (__cdecl *set_t)(ih_t);
typedef ih_t (__cdecl *get_t)(void);
static set_t p_set; static get_t p_get;
static volatile int cg, cl, w_done;
static ih_t volatile w_get;
static void __cdecl hg(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; cg++; }
static void __cdecl hl(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; cl++; }

static DWORD WINAPI worker(LPVOID p)
{
    char d[4]; volatile size_t z = 0; (void)p;
    w_get = p_get();
    strcpy_s(d, z, "a");
    w_done = 1;
    return 0;
}
TC_CASE(worker_isolated)
{
    HANDLE t;
    if (!p_set || !p_get) TC_SKIP("missing");
    cg = cl = 0; w_done = 0; w_get = (ih_t)1;
    p_set(hl);
    CHECK(p_get() == hl);
    t = CreateThread(NULL, 0, worker, NULL, 0, NULL);
    if (t) { WaitForSingleObject(t, 10000); CloseHandle(t); }
    CHECK_EQ_I(w_done, 1);
    CHECK(w_get == NULL);
    CHECK_EQ_I(cg, 1);
    CHECK_EQ_I(cl, 0);
    CHECK(p_get() == hl);
    p_set(NULL);
}
int main(void)
{
    tc_begin();
    p_set = (set_t)tc_crt_proc("_set_thread_local_invalid_parameter_handler");
    p_get = (get_t)tc_crt_proc("_get_thread_local_invalid_parameter_handler");
    _set_invalid_parameter_handler(hg);
    TC_RUN(worker_isolated);
    return tc_end();
}
