/* TC
 * id:    ext_probe_get_iph_after_null
 * area:  exit_signal
 * funcs: _set_invalid_parameter_handler _get_invalid_parameter_handler
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler ("NULL restores default")
 * note:  Probes: after _set_invalid_parameter_handler(NULL), _get_invalid_parameter_handler() returns NULL (default is
 *        represented by a null pointer) rather than some internal default function.
 */
#include "tc.h"
#include <stdlib.h>

static void __cdecl h1(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; }
static _invalid_parameter_handler volatile vh1 = h1;

TC_CASE(null_reads_back_null)
{
    _invalid_parameter_handler orig = _get_invalid_parameter_handler();
    _set_invalid_parameter_handler(vh1);
    _set_invalid_parameter_handler(NULL);
    CHECK_EQ_P(_get_invalid_parameter_handler(), NULL);
    _set_invalid_parameter_handler(orig);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_reads_back_null);
    return tc_end();
}
