/* TC
 * id:    nfx_neg_heap_free_invalid_ptr_probe
 * area:  negative_fatal
 * funcs: free
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.3.3 (free of a pointer not from malloc is undefined)
 * note:  Probes that free() of a stack address / an address inside the image's data section terminates the process
 *        abnormally (exit code neither 0 nor the survived-marker 77). The exact status (0xC0000374 vs 0xC0000005) is
 *        deliberately not asserted; real Windows measurement will tell.
 */
#include "tc.h"
#include <stdlib.h>

static char image_data[256] = {1};

TC_CASE(stack_ptr)
{
    DWORD code = 0; int ran = tc_spawn_self("stack", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(image_data_ptr)
{
    DWORD code = 0; int ran = tc_spawn_self("data", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char local[64]; void *volatile p;
        local[0] = 1;
        p = (c[0] == 's') ? (void *)local : (void *)(image_data + 16);
        free(p);
        return 77;
    }
    TC_RUN(stack_ptr); TC_RUN(image_data_ptr);
    return tc_end();
}
