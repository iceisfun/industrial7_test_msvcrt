/* TC
 * id:    prc_errno_address_per_thread
 * area:  process
 * funcs: _beginthreadex _errno
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.5p3 (thread-local errno); MS docs _errno
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static int *volatile g_w1, *volatile g_w2;
static unsigned __stdcall worker(void *a) { (void)a; g_w1 = _errno(); g_w2 = _errno(); return 0; }

TC_CASE(same_in_thread) { int *a = _errno(), *b = _errno(); CHECK(a != NULL); CHECK_EQ_P(a, b); }
TC_CASE(differs_in_worker)
{
    HANDLE h = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, 0, NULL);
    CHECK(h != NULL); if (!h) return;
    WaitForSingleObject(h, 5000); CloseHandle(h);
    CHECK(g_w1 != NULL);
    CHECK_EQ_P(g_w1, g_w2);
    CHECK(g_w1 != _errno());
}
int main(void) { tc_begin(); TC_RUN(same_in_thread); TC_RUN(differs_in_worker); return tc_end(); }
