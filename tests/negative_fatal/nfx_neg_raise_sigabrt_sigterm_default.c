/* TC
 * id:    nfx_neg_raise_sigabrt_sigterm_default
 * area:  negative_fatal
 * kind:  negative
 * funcs: raise

 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs raise, signal: default action terminates the program with exit code 3

 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, 3);
}
TC_CASE(sigabrt) { run("abrt"); }
TC_CASE(sigterm) { run("term"); }
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        raise(c[0] == 'a' ? SIGABRT : SIGTERM);
        return 77;
    }
    TC_RUN(sigabrt); TC_RUN(sigterm);
    return tc_end();
}
