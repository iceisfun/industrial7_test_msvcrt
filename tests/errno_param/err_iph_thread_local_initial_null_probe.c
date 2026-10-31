/* TC
 * id:    err_iph_thread_local_initial_null_probe
 * area:  errno_param
 * funcs:
 * procs: _get_thread_local_invalid_parameter_handler
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_thread_local_invalid_parameter_handler
 * note:  PROBE: on a fresh thread no thread-local invalid-parameter handler is installed, so the getter returns NULL
 *        (docs do not state the initial value explicitly).
 */
#include "tc.h"

typedef void (__cdecl *ih_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
typedef ih_t (__cdecl *get_t)(void);
static get_t p_get;
static ih_t volatile w_get;
static volatile int w_done;

static DWORD WINAPI worker(LPVOID p) { (void)p; w_get = p_get(); w_done = 1; return 0; }

TC_CASE(fresh_thread_null)
{
    HANDLE t;
    if (!p_get) TC_SKIP("missing");
    w_get = (ih_t)1; w_done = 0;
    t = CreateThread(NULL, 0, worker, NULL, 0, NULL);
    if (t) { WaitForSingleObject(t, 10000); CloseHandle(t); }
    CHECK_EQ_I(w_done, 1);
    CHECK(w_get == NULL);
}
int main(void)
{
    tc_begin();
    p_get = (get_t)tc_crt_proc("_get_thread_local_invalid_parameter_handler");
    TC_RUN(fresh_thread_null);
    return tc_end();
}
