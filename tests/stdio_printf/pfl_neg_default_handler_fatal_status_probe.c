/* TC
 * id:    pfl_neg_default_handler_fatal_status_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vprintf_l _create_locale _free_locale
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter validation"; companion of err_neg_default_handler_fatal
 * note:  PROBE: the exact termination status of _printf_l(NULL) under the default invalid-parameter handler is the fast-fail status
 *        0xC0000409, the same as err_neg_default_handler_fatal measures for strcpy_s.
 */
#include "tc.h"
#include "pfl_shim.h"

static const char * volatile v_nof;

TC_CASE(status_is_fastfail)
{
    DWORD code = 0; int ran = tc_spawn_self("printf_null", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        _locale_t l = _create_locale(LC_ALL, "C");
        if (l) { pl_printf_l(v_nof, l); _free_locale(l); }
        return 77;
    }
    TC_RUN(status_is_fastfail);
    return tc_end();
}
