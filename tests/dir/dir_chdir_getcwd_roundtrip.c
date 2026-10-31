/* TC
 * id:    dir_chdir_getcwd_roundtrip
 * area:  dir
 * funcs: _chdir _getcwd _mkdir _rmdir
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _chdir, _getcwd (returns full path of cwd; NULL buffer allocates)
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static char orig[MAX_PATH];
static const char * volatile sub = "tc_dir_chdir_getcwd_roundtrip_s";

static int ends_with(const char *s, const char *suf)
{
    size_t a = 0, b = 0; while (s[a]) a++; while (suf[b]) b++;
    return a >= b && tc_strequ(s + a - b, suf);
}
static void restore(void) { SetCurrentDirectoryA(orig); }

TC_CASE(chdir_sub)
{
    char c[MAX_PATH], k[MAX_PATH];
    CHECK_EQ_I(_mkdir(sub), 0);
    CHECK_EQ_I(_chdir(sub), 0);
    CHECK(_getcwd(c, MAX_PATH) != NULL);
    CHECK(ends_with(c, "\\tc_dir_chdir_getcwd_roundtrip_s"));
    GetCurrentDirectoryA(MAX_PATH, k);
    CHECK_STR(c, k);
    restore();
}
TC_CASE(parent_and_dot)
{
    char c[MAX_PATH], p[MAX_PATH];
    CHECK_EQ_I(_chdir(sub), 0);
    CHECK_EQ_I(_chdir("."), 0);
    CHECK(_getcwd(c, MAX_PATH) != NULL);
    CHECK(ends_with(c, "\\tc_dir_chdir_getcwd_roundtrip_s"));
    CHECK_EQ_I(_chdir(".."), 0);
    CHECK(_getcwd(p, MAX_PATH) != NULL);
    CHECK_STR(p, orig);
    restore();
}
TC_CASE(normalised)
{
    char c[MAX_PATH];
    CHECK_EQ_I(_chdir("tc_dir_chdir_getcwd_roundtrip_s\\.\\..\\tc_dir_chdir_getcwd_roundtrip_s"), 0);
    CHECK(_getcwd(c, MAX_PATH) != NULL);
    CHECK(ends_with(c, "\\tc_dir_chdir_getcwd_roundtrip_s"));
    { const char *q; int dots = 0; for (q = c; *q; q++) if (q[0]=='\\' && q[1]=='.' && (q[2]=='\\' || q[2]=='.' || q[2]==0)) dots = 1; CHECK(!dots); }
    restore();
}
TC_CASE(getcwd_alloc)
{
    char *p = _getcwd(NULL, 0);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, orig); free(p); }
}
TC_CASE(restored)
{
    char c[MAX_PATH];
    CHECK(_getcwd(c, MAX_PATH) != NULL);
    CHECK_STR(c, orig);
    CHECK_EQ_I(_rmdir(sub), 0);
}
int main(void)
{
    tc_begin();
    GetCurrentDirectoryA(MAX_PATH, orig);
    RemoveDirectoryA("tc_dir_chdir_getcwd_roundtrip_s");
    TC_RUN(chdir_sub); TC_RUN(parent_and_dot); TC_RUN(normalised); TC_RUN(getcwd_alloc); TC_RUN(restored);
    restore(); RemoveDirectoryA("tc_dir_chdir_getcwd_roundtrip_s");
    return tc_end();
}
