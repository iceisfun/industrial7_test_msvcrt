/* TC
 * id:    pfm_1_star_both_negative_width
 * area:  stdio_printf
 * funcs: sprintf _snwprintf _snprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p5 (negative * width = '-' flag then positive width); MS docs printf width
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static char b[64]; static wchar_t w[64];

TC_CASE(sprintf_conv) {
    volatile int wd = -5; volatile int d = 3; volatile int c = 'x'; volatile unsigned x = 255; volatile double f = 1.5;
    sprintf(b, "[%*d]", wd, d); CHECK_STR(b, "[3    ]");
    sprintf(b, "[%*s]", wd, "a"); CHECK_STR(b, "[a    ]");
    wd = -3; sprintf(b, "[%*c]", wd, c); CHECK_STR(b, "[x  ]");
    wd = -10; sprintf(b, "[%*f]", wd, f); CHECK_STR(b, "[1.500000  ]");
    wd = -4; sprintf(b, "[%*x]", wd, x); CHECK_STR(b, "[ff  ]");
    wd = -10; { volatile int p = 2; sprintf(b, "[%*.*f]", wd, p, f); CHECK_STR(b, "[1.50      ]"); }
}
TC_CASE(width_smaller_than_text) {
    volatile int wd = -1; sprintf(b, "[%*s]", wd, "abc"); CHECK_STR(b, "[abc]");
}
TC_CASE(snprintf_conv) {
    volatile int wd = -5; volatile int d = 3; int n;
    n = _snprintf(b, sizeof b, "[%*d][%*s]", wd, d, wd, "a"); CHECK_STR(b, "[3    ][a    ]"); CHECK_EQ_I(n, 14);
}
TC_CASE(wide_conv) {
    volatile int wd = -5; volatile int d = 3; volatile int c = 'x'; volatile double f = 1.5;
    _snwprintf(w, 64, L"[%*d]", wd, d); CHECK_WSTR(w, L"[3    ]");
    _snwprintf(w, 64, L"[%*ls]", wd, L"a"); CHECK_WSTR(w, L"[a    ]");
    wd = -3; _snwprintf(w, 64, L"[%*lc]", wd, (wint_t)c); CHECK_WSTR(w, L"[x  ]");
    wd = -10; _snwprintf(w, 64, L"[%*f]", wd, f); CHECK_WSTR(w, L"[1.500000  ]");
}

int main(void) { tc_begin(); TC_RUN(sprintf_conv); TC_RUN(width_smaller_than_text); TC_RUN(snprintf_conv); TC_RUN(wide_conv); return tc_end(); }
