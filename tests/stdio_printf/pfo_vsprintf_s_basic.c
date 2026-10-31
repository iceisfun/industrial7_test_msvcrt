/* TC
 * id:    pfo_vsprintf_s_basic
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, vsprintf_s ("returns the number of bytes written, not counting the terminating null")
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int calls(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(fits) { char b[32]; int r = calls(0, b, 32, "%d-%d", 12, 34); CHECK_EQ_I(r, 5); CHECK_STR(b, "12-34"); }
TC_CASE(exact_fit) { char b[16]; volatile size_t n = 6; int r = calls(0, b, n, "%d", 12345); CHECK_EQ_I(r, 5); CHECK_STR(b, "12345"); }
TC_CASE(string) { char b[32]; int r = calls(0, b, 32, "<%s|%5s|%-5s|%.2s>", "ab", "cd", "ef", "ghi"); CHECK_STR(b, "<ab|   cd|ef   |gh>"); CHECK_EQ_I(r, 19); }
TC_CASE(empty) { char b[8]; int r; b[0] = 'Z'; r = calls(0, b, 8, ""); CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 0); }
int main(void) { tc_begin(); TC_RUN(fits); TC_RUN(exact_fit); TC_RUN(string); TC_RUN(empty); return tc_end(); }
