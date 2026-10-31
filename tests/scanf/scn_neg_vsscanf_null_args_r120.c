/* TC
 * id:    scn_neg_vsscanf_null_args_r120
 * area:  scanf
 * funcs:
 * procs: vsscanf vswscanf
 * crt:   msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs vsscanf, vswscanf (VS2013+/UCRT): NULL buffer or format invokes the invalid parameter handler, returns EOF, errno EINVAL
 * note:  Validation of NULL args in vsscanf/vswscanf is inferred from the shared scanf parameter validation; not cited from a doc statement.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

typedef int (__cdecl *vss_t)(const char *, const char *, va_list);
typedef int (__cdecl *vsw_t)(const wchar_t *, const wchar_t *, va_list);
static vss_t p_vs; static vsw_t p_vw;

static const char *volatile vs;
static const wchar_t *volatile vw;

static int call_vs(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vs ? p_vs(in, fmt, ap) : EOF - 1; va_end(ap); return r; }
static int call_vw(const wchar_t *in, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vw ? p_vw(in, fmt, ap) : EOF - 1; va_end(ap); return r; }

#define PRE int a = 5, n; CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0
#define POST CHECK_EQ_I(n, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(a, 5)

TC_CASE(null_input)   { PRE; vs = NULL; n = call_vs(vs, "%d", &a); POST; }
TC_CASE(null_format)  { PRE; vs = NULL; n = call_vs("12", vs, &a); POST; }
TC_CASE(wnull_input)  { PRE; vw = NULL; n = call_vw(vw, L"%d", &a); POST; }
TC_CASE(wnull_format) { PRE; vw = NULL; n = call_vw(L"12", vw, &a); POST; }

int main(void) { tc_begin(); p_vs = (vss_t)tc_crt_proc("vsscanf"); p_vw = (vsw_t)tc_crt_proc("vswscanf"); CHECK(p_vs && p_vw); TC_RUN(null_input); TC_RUN(null_format); TC_RUN(wnull_input); TC_RUN(wnull_format); return tc_end(); }
