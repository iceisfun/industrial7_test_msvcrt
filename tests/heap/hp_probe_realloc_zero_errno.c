/* TC
 * id:    hp_probe_realloc_zero_errno
 * area:  heap
 * funcs: realloc malloc
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs realloc
 * note:  PROBING whether errno stays 0 after realloc(p,0) returns NULL (some CRT versions might set an error code on the NULL return).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n;

TC_CASE(errno_untouched)
{
    void *p;
    v_n = 8; p = malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    v_n = 0; errno = 0;
    p = realloc(p, v_n);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, 0);
}
int main(void) { tc_begin(); TC_RUN(errno_untouched); return tc_end(); }
