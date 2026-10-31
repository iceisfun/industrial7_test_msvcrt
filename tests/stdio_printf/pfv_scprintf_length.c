/* TC
 * id:    pfv_scprintf_length
 * area:  stdio_printf
 * funcs: _scprintf _scwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _scwprintf
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(int) { volatile int v = 12345; CHECK_EQ_I(_scprintf("%d", v), 5); }
TC_CASE(string) { volatile const char *s = "hello"; CHECK_EQ_I(_scprintf("[%s]", s), 7); }
TC_CASE(empty) { volatile const char *f = ""; CHECK_EQ_I(_scprintf(f), 0); }
TC_CASE(wide) { volatile const wchar_t *s = L"abc"; CHECK_EQ_I(_scwprintf(L"%s", s), 3); }
TC_CASE(width_pad) { volatile const char *s = "ab"; CHECK_EQ_I(_scprintf("%5s", s), 5); }

int main(void) { tc_begin(); TC_RUN(int); TC_RUN(string); TC_RUN(empty); TC_RUN(wide); TC_RUN(width_pad); return tc_end(); }
