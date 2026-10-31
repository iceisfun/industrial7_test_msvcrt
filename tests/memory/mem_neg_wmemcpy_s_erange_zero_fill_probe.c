/* TC
 * id:    mem_neg_wmemcpy_s_erange_zero_fill_probe
 * area:  memory
 * funcs:
 * procs: wmemcpy_s wmemmove_s
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wmemcpy_s, wmemmove_s (return EINVAL/ERANGE; dest content on failure not stated)
 * note:  PROBE: what happens to dest after wmemcpy_s / wmemmove_s fail with ERANGE or NULL src. The CRT is believed to zero
 *        dest[0..destsz) for wmemcpy_s on failure and to leave dest untouched for wmemmove_s on ERANGE. The log line records the observation;
 *        the only hard assertions are the error code and that wchar_ts beyond destsz are untouched.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *wcp_t)(wchar_t *, size_t, const wchar_t *, size_t);
static wcp_t p_cpy, p_mov;
static const wchar_t src[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

static void log_state(const char *what, const wchar_t *d, int n)
{
    char b[32]; int i; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(what); tc_write(":");
    for (i = 0; i < n; i++) { tc_write(" "); tc_hex(d[i], b); tc_write(b); }
    tc_write("\n");
}
static void run(wcp_t f, const char *nm, int null_src)
{
    wchar_t d[8]; volatile size_t cnt = null_src ? 2 : 6; int i, r;
    CHECK(f != NULL); if (!f) return;
    for (i = 0; i < 8; i++) d[i] = 0x7777;
    tc_iph_count = 0; errno = 0;
    r = f(d, 4, null_src ? NULL : src, cnt);
    CHECK_EQ_I(r, null_src ? EINVAL : ERANGE);
    CHECK_EQ_I(errno, r);
    CHECK_EQ_I(tc_iph_count, 1);
    for (i = 4; i < 8; i++) CHECK_EQ_I(d[i], 0x7777);
    log_state(nm, d, 4);
}
TC_CASE(cpy_erange) { run(p_cpy, "wmemcpy_s erange", 0); }
TC_CASE(cpy_null_src) { run(p_cpy, "wmemcpy_s null src", 1); }
TC_CASE(mov_erange) { run(p_mov, "wmemmove_s erange", 0); }
TC_CASE(mov_null_src) { run(p_mov, "wmemmove_s null src", 1); }
int main(void)
{
    tc_begin();
    p_cpy = (wcp_t)tc_crt_proc("wmemcpy_s"); p_mov = (wcp_t)tc_crt_proc("wmemmove_s");
    tc_iph_install();
    TC_RUN(cpy_erange); TC_RUN(cpy_null_src); TC_RUN(mov_erange); TC_RUN(mov_null_src);
    return tc_end();
}
