/* TC
 * id:    sec_errno_untouched_on_success_probe
 * area:  secure_crt
 * funcs: strcpy_s strcat_s memcpy_s _itoa_s qsort_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.5p3 (no library function sets errno to zero); MS _s docs
 * note:  PROBE: successful _s calls are assumed not to modify errno.
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static int __cdecl cmp(void *ctx, const void *a, const void *b) { (void)ctx; return *(const int *)a - *(const int *)b; }

TC_CASE(set_errno_sentinel_then_call_each_succeeds_errno_unchanged)
{
    char d[16]; char n[16]; int arr[3] = {3, 1, 2};
    errno = 123; CHECK_EQ_I(strcpy_s(d, sizeof d, "ab"), 0); CHECK_EQ_I(errno, 123);
    errno = 123; CHECK_EQ_I(strcat_s(d, sizeof d, "cd"), 0); CHECK_EQ_I(errno, 123);
    errno = 123; CHECK_EQ_I(memcpy_s(n, sizeof n, d, 5), 0); CHECK_EQ_I(errno, 123);
    errno = 123; CHECK_EQ_I(_itoa_s(255, n, sizeof n, 16), 0); CHECK_EQ_I(errno, 123);
    errno = 123; qsort_s(arr, 3, sizeof(int), cmp, NULL); CHECK_EQ_I(errno, 123);
    CHECK_EQ_I(arr[0], 1);
}
int main(void) { tc_begin(); TC_RUN(set_errno_sentinel_then_call_each_succeeds_errno_unchanged); return tc_end(); }
