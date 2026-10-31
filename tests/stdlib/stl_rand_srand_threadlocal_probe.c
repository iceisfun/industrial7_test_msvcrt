/* TC
 * id:    stl_rand_srand_threadlocal_probe
 * area:  stdlib
 * funcs: rand srand
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs srand (seed is per-thread in the multithreaded CRT)
 * note:  Probes that rand state is per-thread: main seeds and draws; a new thread (CreateThread, an exception to rule 6
 *        limited to this probe) must see a fresh seed-1 state (first value 41) and must not disturb main's sequence.
 */
#include "tc.h"
#include <stdlib.h>

static volatile int thread_first = -1, thread_second = -1;
static DWORD WINAPI thr(LPVOID p)
{
    (void)p;
    thread_first = rand();
    thread_second = rand();
    return 0;
}
TC_CASE(per_thread)
{
    HANDLE h; DWORD id; volatile unsigned s = 5; int ref[3], got[3], i;
    srand(s); for (i = 0; i < 3; i++) ref[i] = rand();
    srand(s); got[0] = rand();
    h = CreateThread(NULL, 0, thr, NULL, 0, &id);
    CHECK(h != NULL);
    if (!h) return;
    CHECK(WaitForSingleObject(h, 10000) == WAIT_OBJECT_0);
    CloseHandle(h);
    CHECK_EQ_I(thread_first, 41);
    CHECK_EQ_I(thread_second, 18467);
    got[1] = rand(); got[2] = rand();
    for (i = 0; i < 3; i++) CHECK_EQ_I(got[i], ref[i]);
}
int main(void) { tc_begin(); TC_RUN(per_thread); return tc_end(); }
