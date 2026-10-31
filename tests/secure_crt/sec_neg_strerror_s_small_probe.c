/* TC
 * id:    sec_neg_strerror_s_small_probe
 * area:  secure_crt
 * funcs: strerror_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strerror_s (return value: 0 or error code)
 * note:  Probes strerror_s into a too-small buffer for errno 2 ("No such file or directory"). Believed (CRT copies at most
 *        sizeInBytes-1 characters, then NUL-terminates): the message is silently truncated, the buffer is NUL-terminated,
 *        the return value is 0 and no invalid-parameter handler is needed. Alternatives on real Windows: ERANGE or EINVAL
 *        return. The truncation/termination cases are kept apart from the return-value cases so one guess cannot mask another.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
static volatile size_t vz;
TC_CASE(size_4_truncated_and_terminated)
{ char b[8]; memset(b, 'z', sizeof b); vz = 4; strerror_s(b, vz, 2);
  CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[0], 'N'); CHECK_EQ_I(b[1], 'o'); CHECK_EQ_I(b[2], ' '); }
TC_CASE(size_4_return_zero)
{ char b[8]; int r; memset(b, 'z', sizeof b); vz = 4; r = strerror_s(b, vz, 2); CHECK_EQ_I(r, 0); }
TC_CASE(size_1_empty_string)
{ char b[4]; memset(b, 'z', sizeof b); vz = 1; strerror_s(b, vz, 2); CHECK_EQ_I(b[0], 0); }
TC_CASE(size_1_return_zero)
{ char b[4]; int r; memset(b, 'z', sizeof b); vz = 1; r = strerror_s(b, vz, 2); CHECK_EQ_I(r, 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(size_4_truncated_and_terminated); TC_RUN(size_4_return_zero);
    TC_RUN(size_1_empty_string); TC_RUN(size_1_return_zero);
    return tc_end();
}
