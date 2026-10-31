/* TC
 * id:    pfw_neg_wfopen_bad_ccs
 * area:  stdio_printf
 * kind:  negative
 * funcs: _wfopen
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fopen, _wfopen: ccs=UNICODE|UTF-8|UTF-16LE; other values are invalid (returns NULL, errno EINVAL)
 * note:  Invalid-parameter handler is installed because the path may report through it; its count is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define FN L"tc_pfw_neg_wfopen_bad_ccs_a.tmp"

TC_CASE(bogus)
{ FILE *f; errno = 0; f = _wfopen(FN, L"w, ccs=BOGUS"); CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); if (f) fclose(f); _wunlink(FN); }
TC_CASE(no_value)
{ FILE *f; errno = 0; f = _wfopen(FN, L"w, ccs="); CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); if (f) fclose(f); _wunlink(FN); }
TC_CASE(valid_still_works)
{ FILE *f = _wfopen(FN, L"w, ccs=UTF-8"); CHECK(f != NULL); if (f) fclose(f); _wunlink(FN); }
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(bogus); TC_RUN(no_value); TC_RUN(valid_still_works);
    return tc_end();
}
