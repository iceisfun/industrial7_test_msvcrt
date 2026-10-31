/* TC
 * id:    loc_probe_mbstowcs_1252_undefined_bytes
 * area:  locale
 * funcs: mbstowcs setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS code page 1252 table (bytes 0x81 0x8D 0x8F 0x90 0x9D unassigned)
 * note:  PROBE: MultiByteToWideChar maps these to the same-valued C1 controls; the CRT may instead fail with (size_t)-1/EILSEQ.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static size_t conv1(unsigned char b, wchar_t *w) { char s[2]; s[0] = (char)b; s[1] = 0; w[0] = 0x7777; return mbstowcs(w, s, 1); }
TC_CASE(x81) { wchar_t w[2]; if (!setlocale(LC_ALL, ".1252")) TC_SKIP("no 1252"); CHECK_EQ_U(conv1(0x81, w), 1); CHECK_EQ_U(w[0], 0x81); }
TC_CASE(x8D) { wchar_t w[2]; if (!setlocale(LC_ALL, ".1252")) TC_SKIP("no 1252"); CHECK_EQ_U(conv1(0x8D, w), 1); CHECK_EQ_U(w[0], 0x8D); }
TC_CASE(x8F) { wchar_t w[2]; if (!setlocale(LC_ALL, ".1252")) TC_SKIP("no 1252"); CHECK_EQ_U(conv1(0x8F, w), 1); CHECK_EQ_U(w[0], 0x8F); }
TC_CASE(x90) { wchar_t w[2]; if (!setlocale(LC_ALL, ".1252")) TC_SKIP("no 1252"); CHECK_EQ_U(conv1(0x90, w), 1); CHECK_EQ_U(w[0], 0x90); }
TC_CASE(x9D) { wchar_t w[2]; if (!setlocale(LC_ALL, ".1252")) TC_SKIP("no 1252"); CHECK_EQ_U(conv1(0x9D, w), 1); CHECK_EQ_U(w[0], 0x9D); }

int main(void)
{
    tc_begin();
    TC_RUN(x81); TC_RUN(x8D); TC_RUN(x8F); TC_RUN(x90); TC_RUN(x9D);
    setlocale(LC_ALL, "C");
    return tc_end();
}
