/* TC
 * id:    stl_neg_lfind_invalid
 * area:  stdlib
 * kind:  negative
 * funcs: _lfind
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _lfind "Parameter validation": NULL return, errno EINVAL, invalid parameter handler
 * note:  msvcrt.dll excluded (no parameter validation).
 */
#include "tc.h"
#include <stdlib.h>
#include <search.h>
#include <errno.h>

static int calls;
static int __cdecl cmp(const void *a, const void *b) { calls++; return (a && b) ? *(const int *)a != *(const int *)b : 0; }
static int arr[4] = { 1, 2, 3, 0 };

#define EXPECT_FAIL(call) do { tc_iph_count = 0; errno = 0; calls = 0; \
    CHECK((call) == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0); } while (0)

TC_CASE(lfind_null_key)     { unsigned int n = 3; EXPECT_FAIL(_lfind(NULL, arr, &n, sizeof(int), cmp)); CHECK_EQ_U(n, 3); }
TC_CASE(lfind_null_base)    { int k = 2; unsigned int n = 3; EXPECT_FAIL(_lfind(&k, NULL, &n, sizeof(int), cmp)); }
TC_CASE(lfind_null_num)     { int k = 2; EXPECT_FAIL(_lfind(&k, arr, NULL, sizeof(int), cmp)); }
TC_CASE(lfind_zero_width)   { int k = 2; unsigned int n = 3; EXPECT_FAIL(_lfind(&k, arr, &n, 0, cmp)); }
TC_CASE(lfind_null_compare) { int k = 2; unsigned int n = 3; EXPECT_FAIL(_lfind(&k, arr, &n, sizeof(int), NULL)); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(lfind_null_key); TC_RUN(lfind_null_base); TC_RUN(lfind_null_num); TC_RUN(lfind_zero_width); TC_RUN(lfind_null_compare);
    return tc_end();
}
