/* TC
 * id:    pfs_snprintf_c99_str
 * area:  stdio_printf
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.5; MS docs snprintf (VS2013+ C99 conforming)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>

static volatile const char *vs = "abc";
static volatile const wchar_t *vw = L"abc";

TC_CASE(fits) { char b[16]; int n; memset(b, 'Z', sizeof b); n = snprintf(b, 8, "%s", (const char *)vs); CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); }
TC_CASE(exact) { char b[16]; int n; memset(b, 'Z', sizeof b); n = snprintf(b, 4, "%s", (const char *)vs); CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z'); }
TC_CASE(null_size0) { CHECK_EQ_I(snprintf(NULL, 0, "%s", "hello"), 5); }
TC_CASE(count1) { char b[4]; int n; memset(b, 'Z', sizeof b); n = snprintf(b, 1, "%s", (const char *)vs); CHECK_EQ_I(n, 3); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'Z'); }
TC_CASE(truncate) { char b[8]; int n; memset(b, 'Z', sizeof b); n = snprintf(b, 3, "%s", (const char *)vs); CHECK_EQ_I(n, 3); CHECK_STR(b, "ab"); }
TC_CASE(ls) { char b[16]; int n; n = snprintf(b, 16, "%ls", (const wchar_t *)vw); CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); }
int main(void) { tc_begin(); TC_RUN(fits); TC_RUN(exact); TC_RUN(null_size0); TC_RUN(count1); TC_RUN(truncate); TC_RUN(ls); return tc_end(); }
