/* TC
 * id:    sec_neg_handler_not_called_for_valid
 * area:  secure_crt
 * funcs: strcpy_s memcpy_s _itoa_s _splitpath_s fopen_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s, memcpy_s, _itoa_s, _splitpath_s, fopen_s
 * note:  Negative space: valid but edge-case inputs must succeed and must not invoke the invalid-parameter handler.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static volatile size_t vz = 0;
static volatile size_t v4 = 4;

TC_CASE(empty_src_copy)
{
    char d[4] = "zzz"; int r; tc_iph_count = 0;
    r = strcpy_s(d, 4, "");
    CHECK_EQ_I(r, 0); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 0);
    r = memcpy_s(d, 4, "", vz);   /* zero-length memcpy_s with valid pointers */
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(exact_fit_copy)
{
    char d[4]; int r; tc_iph_count = 0;
    r = strcpy_s(d, v4, "abc");
    CHECK_EQ_I(r, 0); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0);
    r = memcpy_s(d, v4, "xyz", 4);
    CHECK_EQ_I(r, 0); CHECK_STR(d, "xyz"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(zero_value_itoa_s)
{
    char d[2] = "z"; int r; volatile int z = 0; tc_iph_count = 0;
    r = _itoa_s(z, d, 2, 10);
    CHECK_EQ_I(r, 0); CHECK_STR(d, "0"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(fopen_s_missing_file_read)
{
    FILE *f = (FILE *)1; int r; tc_iph_count = 0;
    r = fopen_s(&f, "tc_sec_neg_handler_not_called_missing.tmp", "r");
    CHECK_EQ_I(r, ENOENT); CHECK(f == NULL); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(splitpath_s_null_component_with_size_zero)
{
    char fn[16], ext[8]; int r; tc_iph_count = 0;
    r = _splitpath_s("C:\\dir\\name.txt", NULL, 0, NULL, 0, fn, 16, ext, 8);
    CHECK_EQ_I(r, 0); CHECK_STR(fn, "name"); CHECK_STR(ext, ".txt"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(handler_count_stays_zero)
{
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no handler api\n"); return 1; }
    TC_RUN(empty_src_copy); TC_RUN(exact_fit_copy);
    TC_RUN(zero_value_itoa_s); TC_RUN(fopen_s_missing_file_read);
    TC_RUN(splitpath_s_null_component_with_size_zero); TC_RUN(handler_count_stays_zero);
    return tc_end();
}
