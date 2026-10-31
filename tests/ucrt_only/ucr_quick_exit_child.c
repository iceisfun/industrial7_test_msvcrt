/* TC
 * id:    ucr_quick_exit_child
 * area:  ucrt_only
 * funcs: quick_exit
 * procs: _crt_at_quick_exit _crt_atexit
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.4.7 quick_exit, 7.22.4.3 at_quick_exit; MS docs quick_exit, at_quick_exit
 * note:  at_quick_exit/atexit are static wrappers in the UCRT import library around _crt_at_quick_exit/_crt_atexit.
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *reg_t)(void (__cdecl *)(void));
static const char *mfile = "tc_ucr_quick_exit_child.tmp";

static void mark(char ch)
{
    DWORD n; HANDLE h = CreateFileA(mfile, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, &ch, 1, &n, NULL); CloseHandle(h);
}
static void __cdecl h1(void) { mark('1'); }
static void __cdecl h2(void) { mark('2'); }
static void __cdecl ha(void) { mark('A'); }

static int slurp(char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(mfile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    buf[0] = 0;
    if (h == INVALID_HANDLE_VALUE) return 0;
    ReadFile(h, buf, (DWORD)max - 1, &n, NULL); buf[n] = 0; CloseHandle(h); return 1;
}

static DWORD run(void)
{
    DWORD code = 0; DeleteFileA(mfile);
    CHECK(tc_spawn_self("qe", &code, 20000));
    return code;
}

TC_CASE(exit_code) { CHECK_EQ_U(run(), 9); DeleteFileA(mfile); }
TC_CASE(lifo_and_no_atexit)
{
    char b[16]; run();
    CHECK(slurp(b, sizeof b));
    CHECK_STR(b, "21");   /* at_quick_exit handlers LIFO, atexit handler 'A' absent */
    DeleteFileA(mfile);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        reg_t qr = (reg_t)tc_crt_proc("_crt_at_quick_exit"), ar = (reg_t)tc_crt_proc("_crt_atexit");
        if (!qr || !ar) ExitProcess(90);
        if (ar(ha) != 0) ExitProcess(91);
        if (qr(h1) != 0) ExitProcess(92);
        if (qr(h2) != 0) ExitProcess(93);
        quick_exit(9);
        return 77;
    }
    TC_RUN(exit_code); TC_RUN(lifo_and_no_atexit);
    return tc_end();
}
