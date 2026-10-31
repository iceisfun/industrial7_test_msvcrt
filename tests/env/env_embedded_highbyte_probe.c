/* TC
 * id:    env_embedded_highbyte_probe
 * area:  env
 * funcs: _putenv getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _putenv (multibyte conversion uses the current locale / ANSI code page)
 * note:  Probes whether a narrow value with a non-ASCII byte (0xE9) is accepted in the "C" locale and returned
 *        byte-for-byte by getenv. The conversion to the wide copy may use the ANSI code page or fail; most likely it succeeds.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(high_byte_roundtrip)
{
    const char *v;
    CHECK_EQ_I(_putenv("TC_HB_A=caf\xE9"), 0);
    v = getenv("TC_HB_A");
    CHECK(v != NULL);
    if (v) CHECK_STR(v, "caf\xE9");
    _putenv("TC_HB_A=");
}
int main(void)
{
    tc_begin();
    TC_RUN(high_byte_roundtrip);
    return tc_end();
}
