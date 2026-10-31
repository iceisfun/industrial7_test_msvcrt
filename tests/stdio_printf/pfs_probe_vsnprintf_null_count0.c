/* TC
 * id:    pfs_probe_vsnprintf_null_count0
 * area:  stdio_printf
 * funcs: _vsnprintf _snwprintf
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf/_vsnprintf: NULL buffer with count 0 (UCRT size-query behaviour)
 * note:  PROBE: on ucrtbase _vsnprintf(NULL,0,"%s","abc") and _snwprintf(NULL,0,L"%s",L"abc") return 3 (required length).
 *        Legacy msvcr* are probed in pfs_probe_vsnprintf_null_count0_legacy.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static int vs(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(vsnprintf_len) { tc_iph_install(); CHECK_EQ_I(vs(NULL, 0, "%s", "abc"), 3); }
TC_CASE(snwprintf_len) { tc_iph_install(); CHECK_EQ_I(_snwprintf(NULL, 0, L"%s", L"abc"), 3); }

int main(void) { tc_begin(); TC_RUN(vsnprintf_len); TC_RUN(snwprintf_len); return tc_end(); }
