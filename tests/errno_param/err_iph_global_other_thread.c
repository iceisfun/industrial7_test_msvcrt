/* TC
 * id:    err_iph_global_other_thread
 * area:  errno_param
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler (process-wide); errno is per thread
 * note:  Uses two kernel32 threads, one worker.
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile int cnt;
static volatile int w_ret, w_errno, w_done;
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; cnt++; }

static DWORD WINAPI worker(LPVOID p)
{
    char *volatile d = NULL; (void)p;
    errno = 0;
    w_ret = strcpy_s(d, 4, "a");
    w_errno = errno;
    w_done = 1;
    return 0;
}
static void run_worker(void)
{
    HANDLE t = CreateThread(NULL, 0, worker, NULL, 0, NULL);
    if (t) { WaitForSingleObject(t, 10000); CloseHandle(t); }
}
TC_CASE(handler_fires_in_worker)
{
    cnt = 0; w_done = 0;
    run_worker();
    CHECK_EQ_I(w_done, 1);
    CHECK_EQ_I(cnt, 1);
    CHECK_EQ_I(w_ret, EINVAL);
}
TC_CASE(errno_independent)
{
    errno = ERANGE; w_done = 0;
    run_worker();
    CHECK_EQ_I(w_done, 1);
    CHECK_EQ_I(w_errno, EINVAL);
    CHECK_EQ_I(errno, ERANGE);
    errno = 0;
}
int main(void)
{
    tc_begin();
    _set_invalid_parameter_handler(h);
    TC_RUN(handler_fires_in_worker); TC_RUN(errno_independent);
    return tc_end();
}
