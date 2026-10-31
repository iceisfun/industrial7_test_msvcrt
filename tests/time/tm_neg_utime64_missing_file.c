/* TC
 * id:    tm_neg_utime64_missing_file
 * area:  time
 * funcs: _utime64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _utime: returns -1 and sets errno on failure
 */
#include "tc.h"
#include <sys/utime.h>
#include <time.h>
#include <errno.h>

static struct __utimbuf64 B = { 1000000000, 1100000000 };

TC_CASE(missing)
{
    errno = 0;
    CHECK_EQ_I(_utime64("tc_tm_neg_utime64_missing_file_nonexistent.tmp", &B), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(missing_dir)
{
    errno = 0;
    CHECK_EQ_I(_utime64("/nonexistent_dir/x", &B), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(missing_null_times)
{
    errno = 0;
    CHECK_EQ_I(_utime64("tc_tm_neg_utime64_missing_file_nonexistent.tmp", NULL), -1);
    CHECK_EQ_I(errno, ENOENT);
}
int main(void) { tc_begin(); TC_RUN(missing); TC_RUN(missing_dir); TC_RUN(missing_null_times); return tc_end(); }
