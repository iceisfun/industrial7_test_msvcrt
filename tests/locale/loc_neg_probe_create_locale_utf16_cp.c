/* TC
 * id:    loc_neg_probe_create_locale_utf16_cp
 * area:  locale
 * funcs: _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (code page restrictions)
 * note:  PROBE: _create_locale(LC_ALL, ".1200") (UTF-16LE as locale code page) returns NULL. Believed unsupported but
 *        not verified on every CRT generation.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile n_utf16 = ".1200";

TC_CASE(utf16_cp_null) { _locale_t l = _create_locale(LC_ALL, n_utf16); CHECK(l == NULL); if (l) _free_locale(l); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(utf16_cp_null); return tc_end(); }
