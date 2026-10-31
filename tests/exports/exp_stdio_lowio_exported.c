/* TC
 * id:    exp_stdio_lowio_exported
 * area:  exports
 * funcs: 
 * procs: _open _close _creat _sopen _unlink _read _write _lseek _tell _eof _chsize _filelength _commit _dup _dup2 _pipe _setmode _isatty _umask _access _chmod _fileno _fdopen _fsopen _get_osfhandle _open_osfhandle
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs low-level I/O (io.h)
 * note:  Only underscore names are checked; see exp_neg_plain_posix_names_absent for the opposite.
 */
#include "tc.h"

static void present(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = not exported */
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = unexpectedly exported */
}

static const char *const g_open_close[] = { "_open", "_close", "_creat", "_sopen", "_unlink", NULL };
static const char *const g_read_write_seek[] = { "_read", "_write", "_lseek", "_tell", "_eof", "_chsize", "_filelength", "_commit", NULL };
static const char *const g_dup_pipe[] = { "_dup", "_dup2", "_pipe", NULL };
static const char *const g_mode_attr[] = { "_setmode", "_isatty", "_umask", "_access", "_chmod", NULL };
static const char *const g_handle_conv[] = { "_fileno", "_fdopen", "_fsopen", "_get_osfhandle", "_open_osfhandle", NULL };

TC_CASE(open_close) { present(g_open_close); }
TC_CASE(read_write_seek) { present(g_read_write_seek); }
TC_CASE(dup_pipe) { present(g_dup_pipe); }
TC_CASE(mode_attr) { present(g_mode_attr); }
TC_CASE(handle_conv) { present(g_handle_conv); }

int main(void)
{
    tc_begin();
    TC_RUN(open_close);
    TC_RUN(read_write_seek);
    TC_RUN(dup_pipe);
    TC_RUN(mode_attr);
    TC_RUN(handle_conv);
    return tc_end();
}
