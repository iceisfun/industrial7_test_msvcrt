/* TC
 * id:    tm_neg_utime64_null_path
 * area:  time
 * funcs: _utime64 _wutime64
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _utime (parameter validation convention): NULL filename invokes the invalid parameter handler, errno EINVAL
 */
#include "tc.h"
#include <sys/utime.h>
#include <time.h>
#include <errno.h>

static struct __utimbuf64 B = { 1000000000, 1100000000 };
static const char *volatile np = NULL;
static const wchar_t *volatile wnp = NULL;

TC_CASE(narrow_null)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_utime64(np, &B), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_wutime64(wnp, &B), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(narrow_null); TC_RUN(wide_null); return tc_end(); }
