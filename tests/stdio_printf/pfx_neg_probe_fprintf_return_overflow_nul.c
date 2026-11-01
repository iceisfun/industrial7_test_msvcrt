/* TC
 * id:    pfx_neg_probe_fprintf_return_overflow_nul
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fprintf return value; errno EOVERFLOW
 * note:  OBSERVED on Win10 22H2 19045: an output count exceeding INT_MAX (width 2147483647 plus one more char) makes ucrt fprintf return INT_MIN
 *        (the count wraps; no -1, errno stays 0, no handler), and exactly INT_MAX chars returns INT_MAX. Writes ~2GB to the NUL device,
 *        so each case is a TC_HAZARD (class E: it exceeds the 30 s harness limit; each takes >12 s on the reference VM) and runs in a child
 *        process with a bounded 300 s wait; it reports its elapsed time as TC INFO. Build with TC_HAZARD=1 and a long harness timeout to run it.
 *        A (test bug): r == INT_MIN, errno 0. E: bounded in child.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

static const char *volatile g_over = "%2147483647dx";
static const char *volatile g_exact = "%2147483647d";

/* Each ~2 GB formatted write runs in a child process with a bounded wait (class E: unbounded in the parent). */
static void run_child(const char *name)
{
    DWORD code = 0, t0 = GetTickCount(), dt; int ran; char t[24];
    ran = tc_spawn_self(name, &code, 300000);
    dt = GetTickCount() - t0; tc_i64(dt, t); tc_write("TC INFO " TC_TEST_ID " child " ); tc_write(name); tc_write(" ms="); tc_write(t); tc_write("\n");
    CHECK(ran); CHECK_EQ_U(code, 55);
}
TC_CASE(overflow)      { TC_HAZARD("writes ~2 GB through fprintf; takes longer than the 30 s per-test harness limit"); run_child("overflow"); }
TC_CASE(exact_int_max) { TC_HAZARD("writes ~2 GB through fprintf; takes longer than the 30 s per-test harness limit"); run_child("exact"); }
int main(void)
{
    const char *c;
    tc_begin(); tc_iph_install();
    if ((c = tc_child_name()) != NULL) {
        FILE *f = fopen("NUL", "w"); int r;
        if (!f) return 60;
        tc_iph_install(); tc_iph_count = 0; errno = 0;
        if (c[0] == 'o') { r = fprintf(f, g_over, 1); fclose(f); return (r == INT_MIN && errno == 0 && tc_iph_count == 0) ? 55 : 56; }
        r = fprintf(f, g_exact, 1); fclose(f); return r == INT_MAX ? 55 : 57;
    }
    TC_RUN(overflow); TC_RUN(exact_int_max);
    return tc_end();
}
