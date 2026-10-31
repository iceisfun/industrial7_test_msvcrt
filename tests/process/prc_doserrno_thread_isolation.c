/* TC
 * id:    prc_doserrno_thread_isolation
 * area:  process
 * funcs: _beginthreadex __doserrno
 * procs: _set_doserrno
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _set_doserrno / _doserrno (VS2005+)
 * note:  _set_doserrno exists in msvcr80+; narrowed from all accordingly.
 */
#include "tc.h"
#include <process.h>
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _set_doserrno, (unsigned long));
#define _set_doserrno tc_p__set_doserrno

static unsigned long *volatile worker_ptr;
static volatile unsigned long worker_val;
static unsigned __stdcall worker(void *p)
{
    (void)p;
    _set_doserrno(123);
    worker_ptr = __doserrno();
    worker_val = *__doserrno();
    return 0;
}

TC_CASE(set_returns_zero) { if (!tc_p__set_doserrno) { CHECK(0); return; } CHECK_EQ_I(_set_doserrno(5), 0); CHECK_EQ_U(*__doserrno(), 5); }
TC_CASE(isolation)
{
    HANDLE h; unsigned id;
    if (!tc_p__set_doserrno) { CHECK(0); return; }
    _set_doserrno(5);
    h = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, 0, &id);
    CHECK(h != NULL);
    if (!h) return;
    CHECK_EQ_I(WaitForSingleObject(h, 10000), WAIT_OBJECT_0);
    CloseHandle(h);
    CHECK_EQ_U(worker_val, 123);
    CHECK_EQ_U(*__doserrno(), 5);
    CHECK(worker_ptr != NULL && worker_ptr != __doserrno());
}
int main(void) { tc_begin(); TC_LOAD_PROC(_set_doserrno); TC_RUN(set_returns_zero); TC_RUN(isolation); return tc_end(); }
