/* TC
 * id:    dir_neg_mkdir_long
 * area:  dir
 * funcs: _mkdir _rmdir
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mkdir (returns -1 on error); Win32 naming files (component limit 255)
 * note:  errno is deliberately not asserted here (ENOENT/EINVAL differ); see dir_neg_probe_mkdir_edge_names.
 *        The 'total path > 260' behaviour is in dir_neg_probe_mkdir_total_over_max.
 */
#include "tc.h"
#include <direct.h>

static const char *volatile v_name;
static void fill(char *p, char ch, int n) { int i; for (i = 0; i < n; i++) p[i] = ch; p[n] = 0; }

TC_CASE(component_300)
{
    char name[400], pre[300];
    fill(name, 'm', 300); name[0] = 't'; name[1] = 'c'; name[2] = '_';
    v_name = name;
    CHECK_EQ_I(_mkdir(v_name), -1);
    CHECK_EQ_U(GetFileAttributesA(name), INVALID_FILE_ATTRIBUTES);
    fill(pre, 'm', 255); pre[0] = 't'; pre[1] = 'c'; pre[2] = '_';   /* no truncated directory either */
    CHECK_EQ_U(GetFileAttributesA(pre), INVALID_FILE_ATTRIBUTES);
    pre[254] = 0;
    CHECK_EQ_U(GetFileAttributesA(pre), INVALID_FILE_ATTRIBUTES);
}

TC_CASE(component_256)
{
    char name[400];
    fill(name, 'n', 256); name[0] = 't'; name[1] = 'c'; name[2] = '_';
    v_name = name;
    CHECK_EQ_I(_mkdir(v_name), -1);
    CHECK_EQ_U(GetFileAttributesA(name), INVALID_FILE_ATTRIBUTES);
}

TC_CASE(illegal_chars)
{
    static const char *const names[] = { "tc_dir_neg_a<b", "tc_dir_neg_a>b", "tc_dir_neg_a|b", "tc_dir_neg_a?b", "tc_dir_neg_a*b" };
    int i;
    for (i = 0; i < 5; i++) {
        v_name = names[i];
        CHECK_EQ_I(_mkdir(v_name), -1);
    }
    CHECK_EQ_U(GetFileAttributesA("tc_dir_neg_a"), INVALID_FILE_ATTRIBUTES);
    CHECK_EQ_U(GetFileAttributesA("tc_dir_neg_a<b"), INVALID_FILE_ATTRIBUTES);
}

TC_CASE(rmdir_nonexistent_long)
{
    char name[400];
    fill(name, 'r', 300); name[0] = 't'; name[1] = 'c'; name[2] = '_';
    v_name = name;
    CHECK_EQ_I(_rmdir(v_name), -1);
}

int main(void) { tc_begin(); TC_RUN(component_300); TC_RUN(component_256); TC_RUN(illegal_chars); TC_RUN(rmdir_nonexistent_long); return tc_end(); }
