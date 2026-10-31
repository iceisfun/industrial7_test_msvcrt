/* TC
 * id:    pfw_neg_vscwprintf_p_positional_errors
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vscwprintf_p
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (n$): positions must be a contiguous 1..N set
 * note:  PROBE. Probes that malformed positional formats ( gap %1$d %3$d, position %0$d, the same position used with two
 *        conflicting types ) are rejected with -1 by the length-only wide formatter. errno/iph are only asserted for gap and
 *        zero position. The conflicting-type case passes a valid wide string pointer as the argument so no CRT outcome can crash.
 *        On ucrtbase _scwprintf_p is an inline wrapper over __stdio_common_vswprintf_p (options 6, NULL buffer, count 0).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vscwp(const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_p(6, NULL, 0, f, NULL, ap); }
#else
typedef int (__cdecl *vscwp_t)(const wchar_t *, va_list);
static vscwp_t p_vscwprintf_p;
static int sh_vscwp(const wchar_t *f, va_list ap) { return p_vscwprintf_p ? p_vscwprintf_p(f, ap) : -99; }
#endif
static int scwp(const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vscwp(f, ap); va_end(ap); return r; }
static const wchar_t * volatile f_gap = L"%1$d %3$d";
static const wchar_t * volatile f_zero = L"%0$d";
static const wchar_t * volatile f_conf = L"%1$d %1$s";
static const wchar_t * volatile f_ok = L"%2$d %1$d";
static const wchar_t * volatile warg = L"x";

TC_CASE(control) { CHECK_EQ_I(scwp(f_ok, 1, 22), 4); }
TC_CASE(gap) { int r; tc_iph_count = 0; errno = 0; r = scwp(f_gap, 1, 2, 3); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(zero_position) { int r; tc_iph_count = 0; errno = 0; r = scwp(f_zero, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(conflicting_types) { int r; tc_iph_count = 0; errno = 0; r = scwp(f_conf, warg); CHECK_EQ_I(r, -1); }
TC_CASE(recovers) { (void)scwp(f_gap, 1, 2, 3); CHECK_EQ_I(scwp(f_ok, 1, 22), 4); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vscwprintf_p = (vscwp_t)tc_crt_proc("_vscwprintf_p");
    CHECK(p_vscwprintf_p != NULL);
#endif
    TC_RUN(control); TC_RUN(gap); TC_RUN(zero_position); TC_RUN(conflicting_types); TC_RUN(recovers);
    return tc_end();
}
