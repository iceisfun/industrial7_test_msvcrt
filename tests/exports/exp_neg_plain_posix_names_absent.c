/* TC
 * id:    exp_neg_plain_posix_names_absent
 * area:  exports
 * funcs: 
 * procs: open close read write lseek dup dup2 creat access unlink fileno isatty chmod umask strdup stricmp strnicmp itoa getcwd mkdir
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs: POSIX names are deprecated aliases from oldnames.lib, not DLL exports
 * note:  The CRT DLLs export only the underscore names; the plain POSIX names are supplied statically by oldnames.lib/libmoldname.
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

static const char *const g_fd_names[] = { "open", "close", "read", "write", "lseek", "dup", "dup2", "creat", NULL };
static const char *const g_misc_names[] = { "access", "unlink", "fileno", "isatty", "chmod", "umask", NULL };
static const char *const g_string_names[] = { "strdup", "stricmp", "strnicmp", "itoa", "getcwd", "mkdir", NULL };

TC_CASE(fd_names) { absent(g_fd_names); }
TC_CASE(misc_names) { absent(g_misc_names); }
TC_CASE(string_names) { absent(g_string_names); }

int main(void)
{
    tc_begin();
    TC_RUN(fd_names);
    TC_RUN(misc_names);
    TC_RUN(string_names);
    return tc_end();
}
