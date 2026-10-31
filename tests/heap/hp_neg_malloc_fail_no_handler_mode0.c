/* TC
 * id:    hp_neg_malloc_fail_no_handler_mode0
 * area:  heap
 * kind:  negative
 * funcs: malloc free _set_new_handler _query_new_mode
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_new_mode (mode 0, the default: malloc does not call the new handler on failure)
 * note:  with a handler installed but new mode 0, a failing malloc returns NULL without calling the handler.
 */
#include "tc.h"
#include <stddef.h>
typedef int (__cdecl *tc_pnh)(size_t);
int __cdecl _set_new_mode(int);
int __cdecl _query_new_mode(void);
tc_pnh __cdecl _set_new_handler(tc_pnh);
tc_pnh __cdecl _query_new_handler(void);
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

static int g_calls;
static int __cdecl h(size_t n) { (void)n; g_calls++; return 0; }

TC_CASE(handler_not_called)
{
    volatile size_t sz = SIZE_MAX / 2; void *volatile p; tc_pnh old;
    CHECK_EQ_I(_query_new_mode(), 0);
    g_calls = 0;
    old = _set_new_handler((tc_pnh)h);
    errno = 0; p = malloc(sz);
    _set_new_handler(old);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ENOMEM);
    CHECK_EQ_I(g_calls, 0);
    free(p);
}

int main(void) { tc_begin(); TC_RUN(handler_not_called); return tc_end(); }
