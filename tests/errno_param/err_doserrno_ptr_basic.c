/* TC
 * id:    err_doserrno_ptr_basic
 * area:  errno_param
 * funcs: __doserrno
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _doserrno
 * note:  __doserrno returns unsigned long * to the calling thread's _doserrno.
 */
#include "tc.h"
#include <stdlib.h>

extern unsigned long *__cdecl __doserrno(void);
static unsigned long *volatile other_ptr;

static DWORD WINAPI worker(LPVOID p) { (void)p; other_ptr = __doserrno(); return 0; }

TC_CASE(non_null) { CHECK(__doserrno() != NULL); }
TC_CASE(write_read)
{
    unsigned long *p = __doserrno();
    *p = 0; CHECK_EQ_U(*__doserrno(), 0);
    *p = 5; CHECK_EQ_U(*__doserrno(), 5);
    *p = 0xFFFF; CHECK_EQ_U(*__doserrno(), 0xFFFF);
    *p = 0;
}
TC_CASE(stable) { CHECK_EQ_P(__doserrno(), __doserrno()); }
TC_CASE(per_thread)
{
    HANDLE h = CreateThread(NULL, 0, worker, NULL, 0, NULL);
    CHECK(h != NULL);
    if (h) { WaitForSingleObject(h, 10000); CloseHandle(h); }
    CHECK(other_ptr != NULL);
    CHECK(other_ptr != __doserrno());
}
int main(void)
{
    tc_begin();
    TC_RUN(non_null); TC_RUN(write_read); TC_RUN(stable); TC_RUN(per_thread);
    return tc_end();
}
