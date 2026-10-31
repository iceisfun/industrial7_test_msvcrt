/* TC
 * id:    str_neg_strtok_null_first_call
 * area:  string
 * funcs: strtok
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.24.5.8 (first call with null pointer: behaviour undefined by the standard)
 * note:  PROBE: in a brand-new thread (no prior strtok state) strtok(NULL, ",") returns NULL without crashing. Run in a child process
 *        so a crash or fastfail only shows as a different exit code. The child also installs the invalid-parameter handler, so a
 *        CRT that reports EINVAL through it still passes if it then returns NULL.
 *        Restricted to ucrtbase: older CRTs (msvcrt, msvcr80-120) are believed to dereference the NULL saved token and may crash; add them back only after real-Windows runs show NULL.
 */
#include "tc.h"
#include <string.h>

static volatile const char *vd = ",";
static volatile int result = -1;
static DWORD WINAPI thr(LPVOID p)
{
    char *r; (void)p;
    r = strtok(NULL, (const char *)vd);
    result = r == NULL ? 0 : 1;
    return 0;
}
static int child_main(const char *name)
{
    HANDLE h; (void)name;
    tc_iph_install();
    h = CreateThread(NULL, 0, thr, NULL, 0, NULL);
    if (!h) return 90;
    if (WaitForSingleObject(h, 10000) != WAIT_OBJECT_0) return 91;
    CloseHandle(h);
    return result == 0 ? 77 : 78;
}

TC_CASE(fresh_thread_returns_null)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("nullfirst", &code, 20000));
    CHECK_EQ_U(code, 77);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(fresh_thread_returns_null);
    return tc_end();
}
