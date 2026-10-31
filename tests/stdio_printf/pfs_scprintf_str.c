/* TC
 * id:    pfs_scprintf_str
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _scprintf_l, _scwprintf
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const char *vs = "abc";
static volatile const wchar_t *vw = L"abc";

TC_CASE(plain) { CHECK_EQ_I(_scprintf("%s", (const char *)vs), 3); }
TC_CASE(wide_args) { CHECK_EQ_I(_scprintf("%ls", (const wchar_t *)vw), 3); CHECK_EQ_I(_scprintf("%S", (const wchar_t *)vw), 3); }
TC_CASE(width_prec) { CHECK_EQ_I(_scprintf("%5s", (const char *)vs), 5); CHECK_EQ_I(_scprintf("%.2s", (const char *)vs), 2); }
TC_CASE(char_nul) { volatile int c = 0; CHECK_EQ_I(_scprintf("%c", c), 1); }
TC_CASE(empty) { CHECK_EQ_I(_scprintf(""), 0); }
int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(wide_args); TC_RUN(width_prec); TC_RUN(char_nul); TC_RUN(empty); return tc_end(); }
