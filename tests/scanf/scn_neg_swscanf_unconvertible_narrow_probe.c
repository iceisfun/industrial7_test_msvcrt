/* TC
 * id:    scn_neg_swscanf_unconvertible_narrow_probe
 * area:  scanf
 * kind:  negative
 * funcs: swscanf setlocale
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs swscanf (%hs / %hc store narrow characters converted via the locale code page); wctomb EILSEQ
 * note:  Probes swscanf %hs/%hc with U+20AC / U+0100 in the "C" locale (code points above 0xFF cannot be converted). Believed: conversion fails (ret <= 0), errno EILSEQ, and nothing is written beyond the 2-byte-limited buffer. Whether the destination is untouched or partially written is not asserted beyond a sentinel past the string (b[0] may be untouched, 'a', or a NUL terminator).
 *        Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): in the C locale a wide char that has no narrow form (U+20AC, U+0100) is dropped from %hs/%hc: the conversion still counts (r==1), errno=EILSEQ (42), the destination gets an empty string / NUL
 *        (a lone unconvertible char -> b[0]==0; for L"\x0100x" the string becomes "x"); bytes b[0..4] are zero-filled (5 zero bytes are written even into a 1-byte %hc destination: a Windows overrun on EILSEQ, observed) and b[5..7] stay untouched. The old r<=0 expectation was wrong.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

static volatile const wchar_t *vin;

TC_CASE(hs_euro_c_locale) { char b[8]; int r, i; for (i = 0; i < 8; i++) b[i] = 'Z'; setlocale(LC_ALL, "C"); errno = 0; tc_iph_install(); vin = L"\x20AC"; r = swscanf((const wchar_t *)vin, L"%hs", b); CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 0); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 0); CHECK_EQ_I(b[5], 'Z'); CHECK_EQ_I(b[6], 'Z'); CHECK_EQ_I(b[7], 'Z'); }
TC_CASE(hc_euro_c_locale) { char b[8]; int r, i; for (i = 0; i < 8; i++) b[i] = 'Z'; setlocale(LC_ALL, "C"); errno = 0; tc_iph_install(); vin = L"\x20AC"; r = swscanf((const wchar_t *)vin, L"%hc", b); CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 0); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 0); CHECK_EQ_I(b[5], 'Z'); CHECK_EQ_I(b[6], 'Z'); CHECK_EQ_I(b[7], 'Z'); }
TC_CASE(errno_eilseq) { char b[8]; int r; setlocale(LC_ALL, "C"); errno = 0; tc_iph_install(); vin = L"\x0100x"; r = swscanf((const wchar_t *)vin, L"%hs", b); CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(b[0], 'x'); }
TC_CASE(dest_untouched_or_partial) { char b[8]; int r, i; for (i = 0; i < 8; i++) b[i] = 'Z'; setlocale(LC_ALL, "C"); tc_iph_install(); vin = L"ab\x20AC" L"cd"; r = swscanf((const wchar_t *)vin, L"%hs", b); (void)r; CHECK_EQ_I(b[7], 'Z'); CHECK(b[0] == 'Z' || b[0] == 'a' || b[0] == 0); }

int main(void) { tc_begin(); TC_RUN(hs_euro_c_locale); TC_RUN(hc_euro_c_locale); TC_RUN(errno_eilseq); TC_RUN(dest_untouched_or_partial); return tc_end(); }
