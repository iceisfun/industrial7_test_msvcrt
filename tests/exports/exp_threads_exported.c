/* TC
 * id:    exp_threads_exported
 * area:  exports
 * funcs: 
 * procs: _beginthread _beginthreadex _endthread _endthreadex
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _beginthread, _beginthreadex, _endthread, _endthreadex
 * note:  Each thread routine ends via the matching _endthread/_endthreadex (ends the thread early, before the function returns).
 */
#include "tc.h"

typedef uintptr_t (__cdecl *bt_t)(void (__cdecl *)(void *), unsigned, void *);
typedef uintptr_t (__cdecl *bte_t)(void *, unsigned, unsigned (__stdcall *)(void *), void *, unsigned, unsigned *);
typedef void (__cdecl *et_t)(void);
typedef void (__cdecl *ete_t)(unsigned);

static volatile LONG ran, after_end;

static void __cdecl thread_a(void *arg)
{
    et_t e = (et_t)tc_crt_proc("_endthread");
    InterlockedExchange(&ran, (LONG)(intptr_t)arg);
    if (e) e();
    InterlockedExchange(&after_end, 1);
}
static unsigned __stdcall thread_b(void *arg)
{
    ete_t e = (ete_t)tc_crt_proc("_endthreadex");
    InterlockedExchange(&ran, (LONG)(intptr_t)arg);
    if (e) e(99);
    InterlockedExchange(&after_end, 1);
    return 1;
}
TC_CASE(beginthread)
{
    bt_t b = (bt_t)tc_crt_proc("_beginthread");
    uintptr_t h; int i;
    CHECK(b != NULL);
    if (!b) return;
    ran = 0; after_end = 0;
    h = b(thread_a, 0, (void *)(intptr_t)11);
    CHECK(h != 0 && h != (uintptr_t)-1);
    for (i = 0; i < 500 && ran == 0; i++) Sleep(10);   /* handle is closed by the CRT; poll instead */
    CHECK_EQ_I(ran, 11);
    Sleep(50);
    CHECK_EQ_I(after_end, 0);
}
TC_CASE(beginthreadex)
{
    bte_t b = (bte_t)tc_crt_proc("_beginthreadex");
    uintptr_t h; unsigned id = 0; DWORD code = 0;
    CHECK(b != NULL);
    if (!b) return;
    ran = 0; after_end = 0;
    h = b(NULL, 0, thread_b, (void *)(intptr_t)22, 0, &id);
    CHECK(h != 0);
    if (!h) return;
    CHECK(id != 0);
    CHECK_EQ_U(WaitForSingleObject((HANDLE)h, 10000), WAIT_OBJECT_0);
    GetExitCodeThread((HANDLE)h, &code);
    CloseHandle((HANDLE)h);
    CHECK_EQ_I(ran, 22);
    CHECK_EQ_U(code, 99);
}
TC_CASE(endthread) { CHECK(tc_crt_proc("_endthread") != NULL); CHECK_EQ_I(after_end, 0); }
TC_CASE(endthreadex) { CHECK(tc_crt_proc("_endthreadex") != NULL); CHECK_EQ_I(after_end, 0); }
int main(void)
{
    tc_begin();
    TC_RUN(beginthread); TC_RUN(beginthreadex); TC_RUN(endthread); TC_RUN(endthreadex);
    return tc_end();
}
