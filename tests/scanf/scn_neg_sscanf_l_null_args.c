/* TC
 * id:    scn_neg_sscanf_l_null_args
 * area:  scanf
 * funcs: _sscanf_l _swscanf_l _snscanf_l
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sscanf_l / swscanf_l / _snscanf_l: NULL buffer or format invokes the invalid parameter handler, returns EOF, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

#ifdef _UCRT
/* mingw headers do not give a usable _swscanf_l on ucrt; call the ucrt entry point the way MS headers do */
#include <stdarg.h>
static int tcx_swscanf_l(const wchar_t *s, const wchar_t *f, _locale_t l, ...)
{ va_list ap; int r; va_start(ap, l); r = __stdio_common_vswscanf(_CRT_INTERNAL_LOCAL_SCANF_OPTIONS, s, (size_t)-1, f, l, ap); va_end(ap); return r; }
#undef _swscanf_l
#define _swscanf_l tcx_swscanf_l
#endif

static const char *volatile vs;
static const wchar_t *volatile vw;
static volatile size_t cnt;

#define PRE int a = 5, n; CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; cnt = 4
#define POST CHECK_EQ_I(n, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL)

TC_CASE(sscanf_l_null_input)  { PRE; vs = NULL; n = _sscanf_l(vs, "%d", NULL, &a); POST; CHECK_EQ_I(a, 5); }
TC_CASE(sscanf_l_null_format) { PRE; vs = NULL; n = _sscanf_l("12", vs, NULL, &a); POST; CHECK_EQ_I(a, 5); }
TC_CASE(swscanf_l_null_input) { PRE; vw = NULL; n = _swscanf_l(vw, L"%d", NULL, &a); POST; CHECK_EQ_I(a, 5); }
TC_CASE(swscanf_l_null_format){ PRE; vw = NULL; n = _swscanf_l(L"12", vw, NULL, &a); POST; CHECK_EQ_I(a, 5); }
TC_CASE(snscanf_l_null_input) { PRE; vs = NULL; n = _snscanf_l(vs, cnt, "%d", NULL, &a); POST; CHECK_EQ_I(a, 5); }
TC_CASE(snscanf_l_null_format){ PRE; vs = NULL; n = _snscanf_l("12", cnt, vs, NULL, &a); POST; CHECK_EQ_I(a, 5); }

int main(void)
{
    tc_begin();
    TC_RUN(sscanf_l_null_input); TC_RUN(sscanf_l_null_format);
    TC_RUN(swscanf_l_null_input); TC_RUN(swscanf_l_null_format);
    TC_RUN(snscanf_l_null_input); TC_RUN(snscanf_l_null_format);
    return tc_end();
}
