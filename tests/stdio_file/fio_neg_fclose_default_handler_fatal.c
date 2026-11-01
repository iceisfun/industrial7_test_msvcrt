/* TC
 * id:    fio_neg_fclose_default_handler_fatal
 * area:  stdio_file
 * funcs: fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: with the default invalid-parameter handler the process terminates
 * note:  The exact termination status is deliberately not asserted here (docs promise only termination; see
 *        err_neg_default_handler_fatal for the status probe). Fatal paths run in child processes. Exit code must be non-zero and not 77 (77 = call returned).
 */
#include "tc.h"
#include <stdio.h>


static void chk(const char *name)
{
    DWORD c = 0; int ran = tc_spawn_self(name, &c, 20000);
    CHECK(ran); CHECK(c != 0); CHECK(c != 77);
}
TC_CASE(a) { chk("a"); }
TC_CASE(b) { chk("b"); }
int main(void)
{
    const char *c;
    tc_begin();
    if (tc_child_name()) tc_iph_default();
    if ((c = tc_child_name()) != NULL) {
        FILE *volatile n = NULL;
        if (c[0] == 'b') { _set_invalid_parameter_handler(NULL); }
        fclose(n);
        return 77;
    }
    TC_RUN(a); TC_RUN(b);
    return tc_end();
}
