/* TC
 * id:    pfv_neg_probe_width_precision_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   none documented for numeric overflow of width/precision digits
 * note:  PROBE: a decimal width or precision that does not fit in int ("%2147483648d", "%.2147483648d", "%99999999999s") makes
 *        sprintf fail with -1 (not crash, not wrap to a negative/huge width). errno is probed (EINVAL expected) in separate cases.
 *        Each format runs in a child process (a crash is a failure of that case only); handler installed so an invalid-parameter path returns.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f_w = "%2147483648d";
static const char *volatile f_p = "%.2147483648d";
static const char *volatile f_s = "%99999999999s";
static volatile int one = 1;

/* each format runs in a child process so that a crash is attributed to one case; child exit: 20 = -1 with errno!=EINVAL,
 * 21 = -1 with EINVAL, 30 = any other result */
static int child(const char *c)
{
    char b[64]; int r;
    errno = 0;
    if (c[0] == 'w') r = sprintf(b, f_w, one);
    else if (c[0] == 'p') r = sprintf(b, f_p, one);
    else r = sprintf(b, f_s, "x");
    if (r != -1) return 30;
    return errno == EINVAL ? 21 : 20;
}
static DWORD spawn(const char *n) { DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000); CHECK(ran); return code; }
TC_CASE(width_int_max_plus1) { DWORD c = spawn("w"); CHECK(c == 20 || c == 21); }
TC_CASE(precision_int_max_plus1) { DWORD c = spawn("p"); CHECK(c == 20 || c == 21); }
TC_CASE(width_digits_overflow) { DWORD c = spawn("s"); CHECK(c == 20 || c == 21); }
TC_CASE(errno_width) { CHECK_EQ_U(spawn("w"), 21); }
TC_CASE(errno_precision) { CHECK_EQ_U(spawn("p"), 21); }
TC_CASE(errno_digits) { CHECK_EQ_U(spawn("s"), 21); }

int main(void)
{
    const char *c; tc_begin(); tc_iph_install();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(width_int_max_plus1); TC_RUN(precision_int_max_plus1); TC_RUN(width_digits_overflow);
    TC_RUN(errno_width); TC_RUN(errno_precision); TC_RUN(errno_digits);
    return tc_end();
}
