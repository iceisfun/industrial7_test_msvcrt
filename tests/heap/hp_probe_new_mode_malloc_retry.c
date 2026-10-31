/* TC
 * id:    hp_probe_new_mode_malloc_retry
 * area:  heap
 * funcs: malloc _set_new_mode _set_new_handler free
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_new_mode (with mode 1 malloc calls the new handler on failure)
 * note:  PROBING handler call counts: a failing malloc(SIZE_MAX/2) with mode 1 calls the handler once if it returns 0 (malloc then
 *        returns NULL), and is retried (handler called again) as long as the handler returns nonzero.
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

static int g_calls, g_limit;
static int __cdecl h(size_t n) { (void)n; g_calls++; return g_calls < g_limit; }

TC_CASE(handler_returns_0)
{
    volatile size_t sz = SIZE_MAX / 2; void *volatile p; tc_pnh old; int oldmode;
    g_calls = 0; g_limit = 0;
    oldmode = _set_new_mode(1);
    old = _set_new_handler((tc_pnh)h);
    p = malloc(sz);
    _set_new_handler(old); _set_new_mode(oldmode);
    CHECK(p == NULL);
    CHECK_EQ_I(g_calls, 1);
    free(p);
}
TC_CASE(handler_retry)
{
    volatile size_t sz = SIZE_MAX / 2; void *volatile p; tc_pnh old; int oldmode;
    g_calls = 0; g_limit = 3;   /* returns 1 for calls 1 and 2, 0 on call 3 */
    oldmode = _set_new_mode(1);
    old = _set_new_handler((tc_pnh)h);
    p = malloc(sz);
    _set_new_handler(old); _set_new_mode(oldmode);
    CHECK(p == NULL);
    CHECK_EQ_I(g_calls, 3);
    free(p);
}

int main(void) { tc_begin(); TC_RUN(handler_returns_0); TC_RUN(handler_retry); return tc_end(); }
