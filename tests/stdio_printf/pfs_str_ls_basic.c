/* TC
 * id:    pfs_str_ls_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf type field characters (S, ls, ws)
 * note:  ASCII only to avoid bytes-vs-chars precision ambiguity
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t *volatile wabc = L"abc";
static const wchar_t *volatile wabcd = L"abcd";
static const wchar_t *volatile wempty = L"";

TC_CASE(ls)  { char b[32]; int n = sprintf(b, "[%ls]", wabc); CHECK_STR(b, "[abc]"); CHECK_EQ_I(n, 5); }
TC_CASE(S)   { char b[32]; int n = sprintf(b, "[%S]", wabc);  CHECK_STR(b, "[abc]"); CHECK_EQ_I(n, 5); }
TC_CASE(ws)  { char b[32]; int n = sprintf(b, "[%ws]", wabc); CHECK_STR(b, "[abc]"); CHECK_EQ_I(n, 5); }
TC_CASE(prec) { char b[32]; int n = sprintf(b, "[%.2ls]", wabcd); CHECK_STR(b, "[ab]"); CHECK_EQ_I(n, 4); }
TC_CASE(width) { char b[32]; sprintf(b, "[%6ls][%-6ls]", wabc, wabc); CHECK_STR(b, "[   abc][abc   ]"); }
TC_CASE(empty) { char b[32]; int n = sprintf(b, "[%ls]", wempty); CHECK_STR(b, "[]"); CHECK_EQ_I(n, 2); }

int main(void) { tc_begin(); TC_RUN(ls); TC_RUN(S); TC_RUN(ws); TC_RUN(prec); TC_RUN(width); TC_RUN(empty); return tc_end(); }
