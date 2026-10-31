/* TC
 * id:    scn_neg_swscanf_nonascii_digit_probe
 * area:  scanf
 * kind:  negative
 * funcs: swscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.2 (%d accepts the decimal digits of the subject sequence, i.e. 0-9); MS docs swscanf
 * note:  Probes whether swscanf %d/%x/%f accept non-ASCII digit code points (U+0663 Arabic-Indic 3, U+FF15 fullwidth 5, U+00B2 superscript 2, fullwidth x U+FF58). Believed: no, matching failure with the destination untouched (iswdigit-like classification is not used by the converter).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const wchar_t *vin;

TC_CASE(arabic_indic_d) { int a = 99, r; vin = L"\x0663"; r = swscanf((const wchar_t *)vin, L"%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 99); }
TC_CASE(fullwidth_5_d) { int a = 99, r; vin = L"\xFF15"; r = swscanf((const wchar_t *)vin, L"%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 99); }
TC_CASE(superscript_two_d) { int a = 99, r; vin = L"\x00B2"; r = swscanf((const wchar_t *)vin, L"%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 99); }
TC_CASE(fullwidth_x) { unsigned a = 99; int r; vin = L"0\xFF58" L"1F"; r = swscanf((const wchar_t *)vin, L"%x", &a); CHECK_EQ_I(r, 1); CHECK_EQ_U(a, 0); }
TC_CASE(fullwidth_f) { float f = 99.0f; int r; vin = L"\xFF15.5"; r = swscanf((const wchar_t *)vin, L"%f", &f); CHECK_EQ_I(r, 0); CHECK_BITS_F(f, 99.0f); }

int main(void) { tc_begin(); TC_RUN(arabic_indic_d); TC_RUN(fullwidth_5_d); TC_RUN(superscript_two_d); TC_RUN(fullwidth_x); TC_RUN(fullwidth_f); return tc_end(); }
