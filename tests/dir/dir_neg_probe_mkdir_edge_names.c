/* TC
 * id:    dir_neg_probe_mkdir_edge_names
 * area:  dir
 * funcs: _mkdir _rmdir
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mkdir (errno ENOENT: path not found); Win32 naming files
 * note:  PROBE: asserts only _mkdir("") == -1 with errno ENOENT, and _mkdir of a name with '<' == -1 with errno
 *        ENOENT (ERROR_INVALID_NAME maps to ENOENT). The reserved name "con" and the trailing-dot name are only
 *        recorded (no assertions), with cleanup.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char *volatile v_name;

TC_CASE(empty_string)
{
    v_name = "";
    errno = 0;
    CHECK_EQ_I(_mkdir(v_name), -1);
    CHECK_EQ_I(errno, ENOENT);
}

TC_CASE(illegal_char_errno)
{
    v_name = "tc_dir_probe_a<b";
    errno = 0;
    CHECK_EQ_I(_mkdir(v_name), -1);
    CHECK_EQ_I(errno, ENOENT);
}

TC_CASE(reserved_con_recorded)
{
    int r; v_name = "con";
    r = _mkdir(v_name);
    if (r == 0) {
        char cwd[MAX_PATH], full[MAX_PATH + 16];
        if (GetCurrentDirectoryA(MAX_PATH, cwd) && cwd[1] == ':') {
            lstrcpyA(full, "\\\\?\\"); lstrcatA(full, cwd); lstrcatA(full, "\\con");
            RemoveDirectoryA(full);
        }
    }
    tc_write(r == 0 ? "TC NOTE mkdir(con) succeeded\n" : "TC NOTE mkdir(con) failed\n");
}

TC_CASE(trailing_dot_recorded)
{
    int r; DWORD a;
    v_name = "tc_dir_probe_dot.";
    r = _mkdir(v_name);
    a = GetFileAttributesA("tc_dir_probe_dot");
    tc_write(r == 0 ? "TC NOTE mkdir(dot.) ok; " : "TC NOTE mkdir(dot.) failed; ");
    tc_write(a != INVALID_FILE_ATTRIBUTES ? "name without dot exists\n" : "name without dot absent\n");
    _rmdir("tc_dir_probe_dot."); _rmdir("tc_dir_probe_dot");
}

int main(void)
{
    tc_begin();
    TC_RUN(empty_string); TC_RUN(illegal_char_errno); TC_RUN(reserved_con_recorded); TC_RUN(trailing_dot_recorded);
    return tc_end();
}
