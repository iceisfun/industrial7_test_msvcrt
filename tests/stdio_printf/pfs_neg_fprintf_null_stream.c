/* TC
 * id:    pfs_neg_fprintf_null_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, fwprintf; Parameter validation (NULL stream / format -> invalid parameter, errno EINVAL, return negative)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static FILE * volatile nullf = NULL;
static const char * volatile nullfmt = NULL;
static const char * volatile sfmt = "%s";
static const wchar_t * volatile wsfmt = L"%s";

TC_CASE(fprintf_null_file)
{
    int r; CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    r = fprintf(nullf, sfmt, "x");
    CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fwprintf_null_file)
{
    int r; CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    r = fwprintf(nullf, wsfmt, L"x");
    CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fprintf_null_format)
{
    FILE *f; int r;
    CHECK(tc_iph_install());
    f = fopen("tc_pfs_neg_fprintf_null_stream.tmp", "w");
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = fprintf(f, nullfmt);
    CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove("tc_pfs_neg_fprintf_null_stream.tmp");
}
int main(void) { tc_begin(); TC_RUN(fprintf_null_file); TC_RUN(fwprintf_null_file); TC_RUN(fprintf_null_format); return tc_end(); }
