/* TC
 * id:    hp_set_new_handler_prev
 * area:  heap
 * funcs: _set_new_handler _query_new_handler
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_new_handler, _query_new_handler
 * note:  ucrtbase exports the C-style _set_new_handler(tc_pnh); no handler is installed initially. Returns the previous handler.
 */
#include "tc.h"
#include <stddef.h>
typedef int (__cdecl *tc_pnh)(size_t);
int __cdecl _set_new_mode(int);
int __cdecl _query_new_mode(void);
tc_pnh __cdecl _set_new_handler(tc_pnh);
tc_pnh __cdecl _query_new_handler(void);

static int __cdecl h1(size_t n) { (void)n; return 0; }
static int __cdecl h2(size_t n) { (void)n; return 0; }

TC_CASE(chain)
{
    tc_pnh volatile a = h1, b = h2;
    CHECK(_query_new_handler() == NULL);
    CHECK(_set_new_handler(a) == NULL);
    CHECK(_query_new_handler() == h1);
    CHECK(_set_new_handler(b) == h1);
    CHECK(_query_new_handler() == h2);
    CHECK(_set_new_handler(NULL) == h2);
    CHECK(_query_new_handler() == NULL);
}

int main(void) { tc_begin(); TC_RUN(chain); return tc_end(); }
