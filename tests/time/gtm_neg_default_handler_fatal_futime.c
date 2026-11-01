/* TC
 * id:    gtm_neg_default_handler_fatal_futime
 * area:  time
 * funcs:
 * procs: _futime _futime64 _futime32
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _futime (invalid file descriptor), 'Parameter validation'
 * note:  PROBE (ucrtbase exports _futime64; plain _futime is an inline alias, so falls back to it): _futime(-1, &buf) / _futime32(-1, &buf) with the default invalid-parameter handler. The UCRT default handler
 *        is expected to terminate the process with fast-fail 0xC0000409. Whether _futime reaches the handler for fd -1 (rather than
 *        only returning -1/EBADF) is what is probed. Child exit code 77 = call returned, 78 = export missing.
 *        Children call tc_iph_default(): the mingw startup code installs a no-op invalid-parameter handler, which hid the CRT default.
 */
#include "tc.h"

typedef int (__cdecl *futime64_t)(int, void *);
typedef int (__cdecl *futime32_t)(int, void *);

static void run_child(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 78);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(futime64_badfd) { run_child("futime64"); }
TC_CASE(futime32_badfd) { run_child("futime32"); }
TC_CASE(parent_alive) { CHECK(1); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();  /* mingw startup installs a no-op handler; restore the CRT default (fast-fail) */
        static long long b64[2] = { 1000000000LL, 1000000000LL };
        static int b32[2] = { 1000000000, 1000000000 };
        volatile int fd = -1;
        if (c[6] == '6') {
            futime64_t f = (futime64_t)tc_crt_proc("_futime"); if (!f) f = (futime64_t)tc_crt_proc("_futime64");
            if (!f) return 78;
            f(fd, b64);
        } else {
            futime32_t f = (futime32_t)tc_crt_proc("_futime32");
            if (!f) return 78;
            f(fd, b32);
        }
        return 77;
    }
    TC_RUN(futime64_badfd); TC_RUN(futime32_badfd); TC_RUN(parent_alive);
    return tc_end();
}
