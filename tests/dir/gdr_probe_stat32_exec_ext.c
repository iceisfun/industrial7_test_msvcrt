/* TC
 * id:    gdr_probe_stat32_exec_ext
 * area:  dir
 * funcs: _stat32
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _stat st_mode (_S_IEXEC "execute/search permission, owner")
 * note:  PROBE: the CRT sets _S_IEXEC for files whose extension is .exe, .cmd, .bat or .com (case-insensitive) and for
 *        directories, and leaves it clear for other extensions (.txt). Docs only say 'if file is executable'.
 */
#include "tc.h"
#include <sys/types.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define P "tc_gdr_probe_exec_" TC_S(TC_CRT_VER)
#define DIR_ P "_d.tmp"
static const char *volatile v_path;

static void mk(const char *p)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}
static void cleanup(void)
{
    DeleteFileA(P ".exe"); DeleteFileA(P ".cmd"); DeleteFileA(P ".bat"); DeleteFileA(P ".com");
    DeleteFileA(P ".txt"); DeleteFileA(P ".EXE"); RemoveDirectoryA(DIR_);
}
static int exec_of(const char *p)  /* -1 on stat failure */
{
    struct _stat32 st; v_path = p;
    if (_stat32(v_path, &st) != 0) return -1;
    return (st.st_mode & _S_IEXEC) != 0;
}
TC_CASE(exe) { mk(P ".exe"); CHECK_EQ_I(exec_of(P ".exe"), 1); cleanup(); }
TC_CASE(cmd) { mk(P ".cmd"); CHECK_EQ_I(exec_of(P ".cmd"), 1); cleanup(); }
TC_CASE(bat) { mk(P ".bat"); CHECK_EQ_I(exec_of(P ".bat"), 1); cleanup(); }
TC_CASE(com) { mk(P ".com"); CHECK_EQ_I(exec_of(P ".com"), 1); cleanup(); }
TC_CASE(upper_exe) { mk(P ".EXE"); CHECK_EQ_I(exec_of(P ".EXE"), 1); cleanup(); }
TC_CASE(txt) { mk(P ".txt"); CHECK_EQ_I(exec_of(P ".txt"), 0); cleanup(); }
TC_CASE(dir) { CreateDirectoryA(DIR_, NULL); CHECK_EQ_I(exec_of(DIR_), 1); cleanup(); }
int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(exe); TC_RUN(cmd); TC_RUN(bat); TC_RUN(com); TC_RUN(upper_exe); TC_RUN(txt); TC_RUN(dir);
    cleanup(); return tc_end();
}
