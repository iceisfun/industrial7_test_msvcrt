/* TC
 * id:    str_neg_strtok_null_first_call
 * area:  string
 * funcs: strtok
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.24.5.8 (first call with null pointer: behaviour undefined by the standard)
 * note:  OBSERVED on Win10 22H2 ucrtbase: in a brand-new thread strtok(NULL, ",") dereferences the null per-thread state and
 *        the process dies with an access violation (0xC0000005); it does not return NULL. Run in a child process.
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
    CHECK_EQ_U(code, 0xC0000005u);   /* Class C: Windows crashes (no validation, null saved-pointer deref) */
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(fresh_thread_returns_null);
    return tc_end();
}
