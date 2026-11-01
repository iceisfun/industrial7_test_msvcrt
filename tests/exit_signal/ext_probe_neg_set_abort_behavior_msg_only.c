/* TC
 * id:    ext_probe_neg_set_abort_behavior_msg_only
 * area:  exit_signal
 * funcs: abort
 * procs: _set_abort_behavior _set_error_mode
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _set_abort_behavior, abort
 * note:  With _WRITE_ABORT_MSG on and _CALL_REPORTFAULT off, abort() exits with 3. Class A (observed): the child
 *        redirects the CRT file descriptor 2 to a temp file with _dup2 and writes a 1-byte marker (the old SetStdHandle redirect
 *        also captured 0 bytes, so no redirection defect is claimed). OBSERVED on Win10 22H2 ucrtbase (CREATE_NO_WINDOW child): abort() writes NOTHING
 *        further to fd 2 (file size stays 1), so the old "message on stderr" expectation was wrong. The release UCRT
 *        is believed to emit the abort message only in debug builds [unverified]; a message sent to the Win32 stderr handle (which
 *        _dup2 onto fd 2 also redirects in a console app) would also fail this test.
 */
#include "tc.h"
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FN "tc_ext_probe_abort_msg.tmp"
typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);
static DWORD code = 0xdeadbeef;
static int ran;

static void go(void)
{
    if (ran) return;
    ran = 1; DeleteFileA(FN);
    if (!tc_spawn_self("msg", &code, 20000)) code = 0xdead0002;
}
TC_CASE(exit_code_3) { go(); CHECK_EQ_U(code, 3); }
TC_CASE(stderr_gets_nothing)
{
    WIN32_FILE_ATTRIBUTE_DATA d;
    go();
    CHECK(GetFileAttributesExA(FN, GetFileExInfoStandard, &d));
    CHECK_EQ_U(d.nFileSizeLow, 1);   /* only the 1-byte marker the child wrote itself; abort() added nothing to fd 2 */
    CHECK_EQ_U(d.nFileSizeHigh, 0);
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        int fd;
        int (__cdecl *sem)(int) = (int (__cdecl *)(int))tc_crt_proc("_set_error_mode");
        if (!sab) return 78;
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
        if (sem) sem(1);   /* _OUT_TO_STDERR: avoid a message box */
        fd = _open(FN, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
        if (fd < 0) return 79;
        if (_dup2(fd, 2) != 0) return 80;   /* the CRT writes the abort message through its own fd 2, not through SetStdHandle */
        _write(2, "M", 1);   /* marker proving the redirection works */
        sab(0x1, 0x1 | 0x2);   /* _WRITE_ABORT_MSG on, _CALL_REPORTFAULT off */
        abort();
        return 77;
    }
    TC_RUN(exit_code_3); TC_RUN(stderr_gets_nothing);
    return tc_end();
}
