/* TC
 * id:    sec_neg_asctime_s_small_probe
 * area:  secure_crt
 * funcs: asctime_s _ctime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs asctime_s: buffer must be at least 26 bytes
 * note:  Probes the 25-byte-buffer failure of asctime_s and _ctime64_s: MS docs list EINVAL for sizeInBytes < 26 (kept as probe because
 *        the errno value and the _ctime64_s path are from memory). Whether the invalid-parameter handler runs is not asserted.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
static volatile size_t vsz = 25;
static void mk(struct tm *t)
{ memset(t, 0, sizeof *t); t->tm_year = 100; t->tm_mday = 15; t->tm_wday = 6; t->tm_hour = 1; }
TC_CASE(asctime_s_size_25_error)
{ struct tm t; char b[32]; int r; mk(&t); tc_iph_count = 0; r = asctime_s(b, vsz, &t); CHECK(r != 0); }
TC_CASE(asctime_s_size_25_errno_value)
{ struct tm t; char b[32]; int r; mk(&t); errno = 0; r = asctime_s(b, vsz, &t); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(ctime64_s_size_25_error)
{ __time64_t tt = 1000000000; char b[32]; int r; tc_iph_count = 0; r = _ctime64_s(b, vsz, &tt); CHECK(r != 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(asctime_s_size_25_error); TC_RUN(asctime_s_size_25_errno_value); TC_RUN(ctime64_s_size_25_error);
    return tc_end();
}
