/* TC
 * id:    prc_beginthreadex_many
 * area:  process
 * funcs: _beginthreadex
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _beginthreadex
 */
#include "tc.h"
#include <process.h>

#define N 8
static unsigned __stdcall worker(void *a) { return (unsigned)(ULONG_PTR)a * 2; }

TC_CASE(many)
{
    HANDLE h[N]; unsigned id[N]; int i, j, made = 0;
    for (i = 0; i < N; i++) { id[i] = 0; h[i] = (HANDLE)_beginthreadex(NULL, 0, worker, (void *)(ULONG_PTR)(i + 1), 0, &id[i]); if (h[i]) made++; }
    CHECK_EQ_I(made, N); if (made != N) { for (i = 0; i < N; i++) if (h[i]) CloseHandle(h[i]); return; }
    CHECK_EQ_U(WaitForMultipleObjects(N, h, TRUE, 10000), WAIT_OBJECT_0);
    for (i = 0; i < N; i++) {
        DWORD c = 0; GetExitCodeThread(h[i], &c);
        CHECK_EQ_U(c, (unsigned)(i + 1) * 2);
        for (j = i + 1; j < N; j++) CHECK(id[i] != id[j]);
        CloseHandle(h[i]);
    }
}
int main(void) { tc_begin(); TC_RUN(many); return tc_end(); }
