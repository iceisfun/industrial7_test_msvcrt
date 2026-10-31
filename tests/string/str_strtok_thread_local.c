/* TC
 * id:    str_strtok_thread_local
 * area:  string
 * funcs: strtok
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs strtok: "each thread has its own tokenizing state" (in the multithreaded CRT)
 * note:  Area exception: uses CreateThread. The thread calls only strtok.
 */
#include "tc.h"
#include <string.h>

static char other[] = "p q r";
static volatile int thr_ok;

static DWORD WINAPI thr(LPVOID p)
{
    char *t; (void)p;
    t = strtok(other, " ");
    if (t && tc_strequ(t, "p")) {
        t = strtok(NULL, " ");
        if (t && tc_strequ(t, "q")) thr_ok = 1;
    }
    return 0;
}

TC_CASE(per_thread)
{
    char b[] = "a,b,c";
    HANDLE h; DWORD id;
    CHECK_STR(strtok(b, ","), "a");
    h = CreateThread(NULL, 0, thr, NULL, 0, &id);
    CHECK(h != NULL);
    if (h) { CHECK(WaitForSingleObject(h, 10000) == WAIT_OBJECT_0); CloseHandle(h); }
    CHECK_EQ_I(thr_ok, 1);
    CHECK_STR(strtok(NULL, ","), "b");
    CHECK_STR(strtok(NULL, ","), "c");
    CHECK(strtok(NULL, ",") == NULL);
}

int main(void) { tc_begin(); TC_RUN(per_thread); return tc_end(); }
