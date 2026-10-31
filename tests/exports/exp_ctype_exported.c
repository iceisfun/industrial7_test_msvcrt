/* TC
 * id:    exp_ctype_exported
 * area:  exports
 * funcs: isalpha isdigit isalnum isspace isupper islower ispunct isprint iscntrl isxdigit isgraph tolower toupper iswalpha iswdigit iswalnum iswspace iswupper iswlower iswpunct iswprint iswcntrl iswxdigit iswgraph towlower towupper iswctype _isctype
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4, 7.30
 * note:  Plain export presence.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)
#define HASNOT(n) do { if (tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "unexpectedly exported: " n); } while (0)

/* taking the address makes the linker import each name from the CRT under test */
static void *volatile tc_sink;
static void touch_imports(void)
{
    tc_sink = (void *)isalpha;
    tc_sink = (void *)isdigit;
    tc_sink = (void *)isalnum;
    tc_sink = (void *)isspace;
    tc_sink = (void *)isupper;
    tc_sink = (void *)islower;
    tc_sink = (void *)ispunct;
    tc_sink = (void *)isprint;
    tc_sink = (void *)iscntrl;
    tc_sink = (void *)isxdigit;
    tc_sink = (void *)isgraph;
    tc_sink = (void *)tolower;
    tc_sink = (void *)toupper;
    tc_sink = (void *)iswalpha;
    tc_sink = (void *)iswdigit;
    tc_sink = (void *)iswalnum;
    tc_sink = (void *)iswspace;
    tc_sink = (void *)iswupper;
    tc_sink = (void *)iswlower;
    tc_sink = (void *)iswpunct;
    tc_sink = (void *)iswprint;
    tc_sink = (void *)iswcntrl;
    tc_sink = (void *)iswxdigit;
    tc_sink = (void *)iswgraph;
    tc_sink = (void *)towlower;
    tc_sink = (void *)towupper;
    tc_sink = (void *)iswctype;
    tc_sink = (void *)_isctype;
}


TC_CASE(narrow_classifiers)
{
    HAS("isalpha");
    HAS("isdigit");
    HAS("isalnum");
    HAS("isspace");
    HAS("isupper");
    HAS("islower");
    HAS("ispunct");
    HAS("isprint");
    HAS("iscntrl");
    HAS("isxdigit");
    HAS("isgraph");
}

TC_CASE(wide_classifiers)
{
    HAS("iswalpha");
    HAS("iswdigit");
    HAS("iswalnum");
    HAS("iswspace");
    HAS("iswupper");
    HAS("iswlower");
    HAS("iswpunct");
    HAS("iswprint");
    HAS("iswcntrl");
    HAS("iswxdigit");
    HAS("iswgraph");
}

TC_CASE(case_mapping)
{
    HAS("tolower");
    HAS("toupper");
    HAS("towlower");
    HAS("towupper");
}

TC_CASE(iswctype_isctype)
{
    HAS("iswctype");
    HAS("_isctype");
}

int main(void)
{
    tc_begin();
    touch_imports();
    TC_RUN(narrow_classifiers);
    TC_RUN(wide_classifiers);
    TC_RUN(case_mapping);
    TC_RUN(iswctype_isctype);
    return tc_end();
}
