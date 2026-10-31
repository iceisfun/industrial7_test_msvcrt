/* TC
 * id:    pfs_probe_vsnprintf_null_count0_legacy
 * area:  stdio_printf
 * funcs: _vsnprintf _snwprintf
 * crt:   <140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf/_vsnprintf: NULL buffer with count 0 (UCRT size-query behaviour)
 * note:  PROBE: on legacy msvcr* the same calls return -1 (no size query; output cannot be stored in a zero-count stream).
 *        ucrtbase is probed in pfs_probe_vsnprintf_null_count0. Guess: -1 (could be 3 or invalid-parameter).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static int vs(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(vsnprintf_len) { tc_iph_install(); CHECK_EQ_I(vs(NULL, 0, "%s", "abc"), -1); }
TC_CASE(snwprintf_len) { tc_iph_install(); CHECK_EQ_I(_snwprintf(NULL, 0, L"%s", L"abc"), -1); }

int main(void) { tc_begin(); TC_RUN(vsnprintf_len); TC_RUN(snwprintf_len); return tc_end(); }
