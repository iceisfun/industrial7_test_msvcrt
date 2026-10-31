/* TC
 * id:    mem_neg_secure_default_handler_fatal
 * area:  memory
 * kind:  negative
 * funcs:
 * procs: memcpy_s memmove_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation" / memcpy_s: with the default invalid-parameter handler the process terminates
 * note:  Fatal paths run in child processes. Only asserts abnormal termination (code not 0/77/78/79; 78 = proc missing, 79 = control failed), not the exact status.
 *        child_not_reached_77 is a control: a valid memcpy_s in the child must let it reach "return 77".
 */
#include "tc.h"

typedef int (__cdecl *cp_t)(void *, size_t, const void *, size_t);
static volatile char sbuf[8] = "abcdefg", dbuf[8];

static int child_main(const char *n)
{
    cp_t cpy = (cp_t)tc_crt_proc("memcpy_s"), mov = (cp_t)tc_crt_proc("memmove_s");
    if (!cpy || !mov) return 78;
    if (tc_strequ(n, "cp_null_dest")) cpy(NULL, 8, (const void *)sbuf, 4);
    else if (tc_strequ(n, "cp_range")) cpy((void *)dbuf, 2, (const void *)sbuf, 4);
    else if (tc_strequ(n, "mv_null_src")) mov((void *)dbuf, 8, NULL, 4);
    else if (tc_strequ(n, "ok")) { if (cpy((void *)dbuf, 8, (const void *)sbuf, 4) != 0) return 79; }
    return 77;
}
static void run(const char *n, int expect_fatal)
{
    DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran);
    if (expect_fatal) { CHECK(code != 0); CHECK(code != 77); CHECK(code != 78); CHECK(code != 79); }
    else CHECK_EQ_U(code, 77);
}
TC_CASE(child_memcpy_s_null_dest_terminates) { run("cp_null_dest", 1); }
TC_CASE(child_memcpy_s_range_terminates) { run("cp_range", 1); }
TC_CASE(child_memmove_s_null_src_terminates) { run("mv_null_src", 1); }
TC_CASE(child_not_reached_77) { run("ok", 0); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_memcpy_s_null_dest_terminates); TC_RUN(child_memcpy_s_range_terminates);
    TC_RUN(child_memmove_s_null_src_terminates); TC_RUN(child_not_reached_77);
    return tc_end();
}
