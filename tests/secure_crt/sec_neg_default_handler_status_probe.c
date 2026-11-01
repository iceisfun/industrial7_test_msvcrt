/* TC
 * id:    sec_neg_default_handler_status_probe
 * area:  secure_crt
 * funcs: strcpy_s memcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs Parameter validation (termination only)
 * note:  Observed on Win10 22H2 19045 ucrtbase: with the CRT's default invalid-parameter handler restored in the child
 *        (tc_iph_default(); the mingw startup installs a no-op handler that made the child just return 77) strcpy_s(d, 0, "x")
 *        and memcpy_s(d, 8, src, 16) ends the process with STATUS_STACK_BUFFER_OVERRUN 0xC0000409 (fast-fail). Stable over
 *        repeated runs. Fatal path runs in a child process.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
static volatile size_t v0 = 0, v8 = 8;
static int child_main(const char *n)
{
    char d[8] = "ab";
    tc_iph_default();
    if (tc_strequ(n, "strcpy")) strcpy_s(d, v0, "x");
    else if (tc_strequ(n, "memcpy")) memcpy_s(d, v8, "0123456789abcdef", 16);
    return 77;
}
static DWORD run(const char *n) { DWORD code = 77; int ran = tc_spawn_self(n, &code, 20000); CHECK(ran); return code; }
static void chk(DWORD c)
{
#if TC_CRT_VER >= 140   /* tc.py defines _UCRT, never TC_UCRT, so the old #ifdef TC_UCRT always took the relaxed branch */
    CHECK_EQ_U(c, TC_STATUS_FASTFAIL);
#else
    CHECK(c == TC_STATUS_FASTFAIL || c == TC_STATUS_INVALID_CRT_PARAM);
#endif
}
TC_CASE(strcpy_s_status) { chk(run("strcpy")); }
TC_CASE(memcpy_s_status) { chk(run("memcpy")); }
TC_CASE(status_not_0xC0000005) { CHECK(run("strcpy") != TC_STATUS_ACCESS_VIOLATION); CHECK(run("memcpy") != TC_STATUS_ACCESS_VIOLATION); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(strcpy_s_status); TC_RUN(memcpy_s_status); TC_RUN(status_not_0xC0000005);
    return tc_end();
}
