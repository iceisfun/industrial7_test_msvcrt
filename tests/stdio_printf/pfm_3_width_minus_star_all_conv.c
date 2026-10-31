/* TC
 * id:    pfm_3_width_minus_star_all_conv
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p5 (negative field width argument is a - flag); MS docs printf width specification
 * note:  star argument handling per C11; MS docs describe the * width/precision forms only loosely
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define N(x) (sizeof(x) / sizeof((x)[0]))
static volatile double d = 1.5;
static volatile int u = 42;

TC_CASE(integers)
{
    char b[128]; volatile unsigned a = 42, o = 8, x = 255; volatile int i = 7;
    sprintf(b, "[%*u][%*o][%*x][%*X][%*i]", -6, a, -6, o, -6, x, -6, x, -6, i);
    CHECK_STR(b, "[42    ][10    ][ff    ][FF    ][7     ]");
}
TC_CASE(chars_strings)
{
    char b[128]; volatile int c = 'x'; volatile wchar_t wc = L'z';
    static const char s[] = "ab"; static const wchar_t ws[] = L"cd";
    sprintf(b, "[%*c][%*s][%*S][%*C]", -4, c, -4, s, -4, ws, -4, (int)wc);
    CHECK_STR(b, "[x   ][ab  ][cd  ][z   ]");
}
TC_CASE(floats)
{
    char b[128];
    sprintf(b, "[%*f][%*g]", -12, d, -12, d);
    CHECK_STR(b, "[1.500000    ][1.5         ]");
}
TC_CASE(float_e)
{
    char b[128];
    sprintf(b, "[%*.1e]", -12, d);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "[1.5e+00     ]");
#else
    CHECK_STR(b, "[1.5e+000    ]");
#endif
}
TC_CASE(float_a)
{
#if TC_AT_LEAST(120)
    char b[128];
    sprintf(b, "[%*.1a]", -12, d);
    CHECK_STR(b, "[0x1.8p+0    ]");
#else
    TC_SKIP("no %a");
#endif
}
TC_CASE(wide)
{
    wchar_t b[128]; volatile unsigned x = 255; volatile int c = 'x';
    static const wchar_t ws[] = L"ab"; static const char s[] = "cd";
    _snwprintf(b, N(b), L"[%*x][%*c][%*s][%*S][%*f]", -6, x, -4, c, -4, ws, -4, s, -12, d);
    CHECK_WSTR(b, L"[ff    ][x   ][ab  ][cd  ][1.500000    ]");
}
int main(void) { tc_begin(); TC_RUN(integers); TC_RUN(chars_strings); TC_RUN(floats); TC_RUN(float_e); TC_RUN(float_a); TC_RUN(wide); return tc_end(); }
