/* TC
 * id:    scn_neg_swscanf_unconvertible_narrow_probe
 * area:  scanf
 * kind:  negative
 * funcs: swscanf setlocale
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs swscanf (%hs / %hc store narrow characters converted via the locale code page); wctomb EILSEQ
 * note:  Probes swscanf %hs/%hc with U+20AC / U+0100 in the "C" locale (code points above 0xFF cannot be converted). Believed: conversion fails (ret <= 0), errno EILSEQ, and nothing is written beyond the 2-byte-limited buffer. Whether the destination is untouched or partially written is not asserted beyond a sentinel past the string (b[0] may be untouched, 'a', or a NUL terminator).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

static volatile const wchar_t *vin;

TC_CASE(hs_euro_c_locale) { char b[8]; int r, i; for (i = 0; i < 8; i++) b[i] = 'Z'; setlocale(LC_ALL, "C"); errno = 0; tc_iph_install(); vin = L"\x20AC"; r = swscanf((const wchar_t *)vin, L"%hs", b); CHECK(r <= 0); CHECK_EQ_I(b[4], 'Z'); CHECK_EQ_I(b[7], 'Z'); }
TC_CASE(hc_euro_c_locale) { char b[8]; int r, i; for (i = 0; i < 8; i++) b[i] = 'Z'; setlocale(LC_ALL, "C"); errno = 0; tc_iph_install(); vin = L"\x20AC"; r = swscanf((const wchar_t *)vin, L"%hc", b); CHECK(r <= 0); CHECK_EQ_I(b[1], 'Z'); CHECK_EQ_I(b[7], 'Z'); }
TC_CASE(errno_eilseq) { char b[8]; int r; setlocale(LC_ALL, "C"); errno = 0; tc_iph_install(); vin = L"\x0100x"; r = swscanf((const wchar_t *)vin, L"%hs", b); CHECK(r <= 0); CHECK_EQ_I(errno, EILSEQ); }
TC_CASE(dest_untouched_or_partial) { char b[8]; int r, i; for (i = 0; i < 8; i++) b[i] = 'Z'; setlocale(LC_ALL, "C"); tc_iph_install(); vin = L"ab\x20AC" L"cd"; r = swscanf((const wchar_t *)vin, L"%hs", b); (void)r; CHECK_EQ_I(b[7], 'Z'); CHECK(b[0] == 'Z' || b[0] == 'a' || b[0] == 0); }

int main(void) { tc_begin(); TC_RUN(hs_euro_c_locale); TC_RUN(hc_euro_c_locale); TC_RUN(errno_eilseq); TC_RUN(dest_untouched_or_partial); return tc_end(); }
