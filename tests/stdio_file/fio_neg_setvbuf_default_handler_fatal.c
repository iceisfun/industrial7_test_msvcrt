/* TC
 * id:    fio_neg_setvbuf_default_handler_fatal
 * area:  stdio_file
 * funcs: setvbuf fopen fclose remove
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setvbuf ("if mode or size is not valid, the invalid parameter handler is invoked"); MS docs "Parameter validation"
 * note:  Without a handler installed an invalid parameter terminates the process abnormally. Each fatal call runs in its own
 *        child process; the child returns 77 only if the CRT failed to terminate. Only "terminated, not 0, not 77" is asserted.
 */
#include "tc.h"
#include <stdio.h>

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}

TC_CASE(setvbuf_bad_mode) { run("badmode"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'b') {
            volatile int mode = 99; FILE *fp = fopen("tc_fio_neg_setvbuf_default_handler_fatal_c.tmp", "w");
            if (fp) setvbuf(fp, NULL, mode, 10);
            if (fp) fclose(fp);
            remove("tc_fio_neg_setvbuf_default_handler_fatal_c.tmp");
        }
        return 77;
    }
    TC_RUN(setvbuf_bad_mode);
    return tc_end();
}
