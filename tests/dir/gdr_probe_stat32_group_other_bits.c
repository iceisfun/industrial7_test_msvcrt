/* TC
 * id:    gdr_probe_stat32_group_other_bits
 * area:  dir
 * funcs: _stat32
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _stat st_mode (documents only the user bits)
 * note:  PROBE: the CRT replicates the user permission bits into the group and other positions, i.e.
 *        (mode & 0070) == ((mode & 0700) >> 3) and (mode & 0007) == ((mode & 0700) >> 6), for a normal file,
 *        a read-only file and a directory.
 */
#include "tc.h"
#include <sys/types.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define F "tc_gdr_probe_grp_f" TC_S(TC_CRT_VER) ".tmp"
#define DIR_ "tc_gdr_probe_grp_d" TC_S(TC_CRT_VER) ".tmp"
static const char *volatile v_path;

static void cleanup(void) { SetFileAttributesA(F, FILE_ATTRIBUTE_NORMAL); DeleteFileA(F); RemoveDirectoryA(DIR_); }
static void check_rep(const char *p)
{
    struct _stat32 st; int r = _stat32(p, &st); unsigned m;
    CHECK_EQ_I(r, 0);
    if (r) return;
    m = st.st_mode;
    CHECK_EQ_U(m & 0070, (m & 0700) >> 3);
    CHECK_EQ_U(m & 0007, (m & 0700) >> 6);
}
TC_CASE(normal_file)
{
    HANDLE h = CreateFileA(F, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    v_path = F; check_rep(v_path); cleanup();
}
TC_CASE(readonly_file)
{
    HANDLE h = CreateFileA(F, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    SetFileAttributesA(F, FILE_ATTRIBUTE_READONLY);
    v_path = F; check_rep(v_path); cleanup();
}
TC_CASE(directory) { CreateDirectoryA(DIR_, NULL); v_path = DIR_; check_rep(v_path); cleanup(); }
int main(void) { tc_begin(); cleanup(); TC_RUN(normal_file); TC_RUN(readonly_file); TC_RUN(directory); cleanup(); return tc_end(); }
