/* TC
 * id:    pfx_neg_fopen_missing_path
 * area:  stdio_printf
 * kind:  negative
 * funcs: fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (returns NULL on error; errno ENOENT for file or path not found)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile missing = "tc_pfx_neg_fopen_missing_nofile.tmp";
static const char *volatile nodir = "tc_pfx_neg_fopen_missing_nodir\\x.tmp";

TC_CASE(read_missing)
{
    FILE *f; errno = 0; f = fopen(missing, "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); if (f) fclose(f);
}
TC_CASE(write_in_missing_dir)
{
    FILE *f; errno = 0; f = fopen(nodir, "w");
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); if (f) fclose(f);
}
TC_CASE(rplus_missing)
{
    FILE *f; errno = 0; f = fopen(missing, "r+");
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); if (f) fclose(f);
}
TC_CASE(no_side_effect)
{
    CHECK(GetFileAttributesA(missing) == INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    TC_RUN(read_missing); TC_RUN(write_in_missing_dir); TC_RUN(rplus_missing); TC_RUN(no_side_effect);
    return tc_end();
}
