/* TC
 * id:    dir_neg_mkdir_parent_missing
 * area:  dir
 * kind:  negative
 * funcs: _mkdir
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mkdir: "ENOENT: Path not found" (_mkdir creates only one directory level at a time)
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char * volatile deep = "tc_dir_neg_mkdir_parent_missing_a\\b\\c";

TC_CASE(parent_missing)
{
    errno = 0;
    CHECK_EQ_I(_mkdir(deep), -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK(GetFileAttributesA("tc_dir_neg_mkdir_parent_missing_a") == INVALID_FILE_ATTRIBUTES);
    CHECK(GetFileAttributesA(deep) == INVALID_FILE_ATTRIBUTES);
}
TC_CASE(one_level_only)
{
    /* a exists, a\b missing: a\b\c still fails and a\b is not created */
    CHECK_EQ_I(_mkdir("tc_dir_neg_mkdir_parent_missing_a"), 0);
    errno = 0;
    CHECK_EQ_I(_mkdir(deep), -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK(GetFileAttributesA("tc_dir_neg_mkdir_parent_missing_a\\b") == INVALID_FILE_ATTRIBUTES);
    CHECK_EQ_I(_rmdir("tc_dir_neg_mkdir_parent_missing_a"), 0);
}
int main(void)
{
    tc_begin();
    RemoveDirectoryA("tc_dir_neg_mkdir_parent_missing_a");
    TC_RUN(parent_missing); TC_RUN(one_level_only);
    return tc_end();
}
