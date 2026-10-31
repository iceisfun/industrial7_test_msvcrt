/* TC
 * id:    sec_neg_small_buffer_probe
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: tmpnam_s strerror_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs tmpnam_s, strerror_s (secure-CRT buffer-size conventions)
 * note:  Probes the return code when the buffer is too small to hold the result: tmpnam_s with a 4-char buffer (guess ERANGE=34) and
 *        strerror_s("Invalid argument") into a 5-byte buffer (case 3: buffer is NUL-terminated within its size, return
 *        value not asserted; case 4: guess - CRT truncates to "Inva" and returns 0 rather than ERANGE).
 *        tmpnam_s with size 0 (guess EINVAL=22).
 *        Each guess is its own case.
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *tmpnam_s_t)(char *, size_t);
typedef int (__cdecl *se_t)(char *, size_t, int);
static tmpnam_s_t p_tmpnam_s; static se_t p_se;

TC_CASE(tmpnam_s_size_zero_EINVAL) { char b[8] = "x"; int r; tc_iph_count = 0; r = p_tmpnam_s(b, 0); CHECK_EQ_I(r, 22); }
TC_CASE(tmpnam_s_4_char_buffer_ERANGE) { char b[4]; int r; tc_iph_count = 0; r = p_tmpnam_s(b, sizeof b); CHECK_EQ_I(r, 34); }
TC_CASE(strerror_s_5_byte_buffer_nul_terminated)
{
    char b[5]; int r, i, nul = 0; tc_iph_count = 0;
    for (i = 0; i < 5; i++) b[i] = 'Q';
    r = p_se(b, sizeof b, 22); (void)r;
    for (i = 0; i < 5; i++) if (!b[i]) nul = 1;
    CHECK(nul);
}
TC_CASE(strerror_s_5_byte_buffer_truncates_returns_0)
{
    char b[5]; int r; tc_iph_count = 0;
    r = p_se(b, sizeof b, 22); CHECK_EQ_I(r, 0); CHECK_STR(b, "Inva");
}
int main(void)
{
    tc_begin();
    p_tmpnam_s = (tmpnam_s_t)tc_crt_proc("tmpnam_s"); p_se = (se_t)tc_crt_proc("strerror_s");
    CHECK(p_tmpnam_s && p_se); if (!(p_tmpnam_s && p_se)) return tc_end();
    tc_iph_install();
    TC_RUN(tmpnam_s_size_zero_EINVAL); TC_RUN(tmpnam_s_4_char_buffer_ERANGE); TC_RUN(strerror_s_5_byte_buffer_nul_terminated); TC_RUN(strerror_s_5_byte_buffer_truncates_returns_0);
    return tc_end();
}
