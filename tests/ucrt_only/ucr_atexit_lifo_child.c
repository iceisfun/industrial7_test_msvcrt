/* TC
 * id:    ucr_atexit_lifo_child
 * area:  ucrt_only
 * funcs:
 * procs: atexit
 * crt:   win7
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.22.4.2 atexit; MS docs atexit (returns 0 on success)
 * note:  Children register A,B,C via atexit. One returns from main, the other calls exit(0). Both must give "CBA".
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *atexit_t)(void (__cdecl *)(void));
static const char *marker = "tc_ucr_atexit_lifo_child.tmp";

static void put(char ch)
{
    HANDLE h = CreateFileA(marker, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD n;
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, &ch, 1, &n, NULL); CloseHandle(h);
}
static void hA(void) { put('A'); }
static void hB(void) { put('B'); }
static void hC(void) { put('C'); }

static void run(const char *name)
{
    DWORD code = 99, n = 0; char buf[32]; HANDLE h;
    DeleteFileA(marker);
    CHECK(tc_spawn_self(name, &code, 20000));
    CHECK_EQ_U(code, 0);
    buf[0] = 0;
    h = CreateFileA(marker, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, buf, 31, &n, NULL); CloseHandle(h); buf[n] = 0; }
    CHECK_STR(buf, "CBA");
    DeleteFileA(marker);
}
TC_CASE(return_from_main) { run("ret"); }
TC_CASE(exit_zero) { run("exit"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile int r = 0; atexit_t ae = (atexit_t)tc_crt_proc("atexit");
        if (!ae) return 79;
        r |= ae(hA); r |= ae(hB); r |= ae(hC);
        if (r) return 78;
        if (c[0] == 'e') exit(0);
        return 0;
    }
    TC_RUN(return_from_main); TC_RUN(exit_zero);
    return tc_end();
}
