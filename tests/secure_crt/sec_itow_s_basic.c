/* TC
 * id:    sec_itow_s_basic
 * area:  secure_crt
 * funcs: _itow_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _itoa_s, _itow_s

 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
static wchar_t b[40];
static volatile int v;
TC_CASE(radix10) { v = 2024; CHECK_EQ_I(_itow_s(v, b, 40, 10), 0); CHECK_WSTR(b, L"2024"); }
TC_CASE(negative) { v = -77; CHECK_EQ_I(_itow_s(v, b, 40, 10), 0); CHECK_WSTR(b, L"-77"); }
TC_CASE(radix16) { v = 0xbeef; CHECK_EQ_I(_itow_s(v, b, 40, 16), 0); CHECK_WSTR(b, L"beef"); }
TC_CASE(radix2) { v = 10; CHECK_EQ_I(_itow_s(v, b, 40, 2), 0); CHECK_WSTR(b, L"1010"); }
TC_CASE(exact_fit) { wchar_t e[4]; v = 999; CHECK_EQ_I(_itow_s(v, e, 4, 10), 0); CHECK_WSTR(e, L"999"); }
int main(void) { tc_begin(); TC_RUN(radix10); TC_RUN(negative); TC_RUN(radix16); TC_RUN(radix2); TC_RUN(exact_fit); return tc_end(); }
