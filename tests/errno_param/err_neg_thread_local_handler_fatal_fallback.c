/* TC
 * id:    err_neg_thread_local_handler_fatal_fallback
 * area:  errno_param
 * funcs: strcpy_s
 * procs: _set_thread_local_invalid_parameter_handler
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_thread_local_invalid_parameter_handler
 * note:  PROBE: a thread-local handler set on the main thread is not inherited by a worker thread, so a bad call there takes the default fatal path.
 */
#include "tc.h"
#include <string.h>

typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
TC_DECL_PROC(iph_t, _set_thread_local_invalid_parameter_handler, (iph_t));

static DWORD WINAPI worker(LPVOID p)
{
    volatile size_t n = 0; char d[4]; (void)p;
    strcpy_s(d, n, "x");
    return 0;
}

TC_CASE(worker_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("worker", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}

int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_set_thread_local_invalid_parameter_handler);
    if ((c = tc_child_name()) != NULL) {
        HANDLE h;
        if (!tc_p__set_thread_local_invalid_parameter_handler) return 3;
        tc_p__set_thread_local_invalid_parameter_handler(tc_iph);
        h = CreateThread(NULL, 0, worker, NULL, 0, NULL);
        if (!h) return 4;
        WaitForSingleObject(h, 10000);
        return 77;
    }
    if (!tc_p__set_thread_local_invalid_parameter_handler) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(worker_terminates);
    return tc_end();
}
