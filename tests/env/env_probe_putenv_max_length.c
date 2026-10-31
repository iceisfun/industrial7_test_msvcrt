/* TC
 * id:    env_probe_putenv_max_length
 * area:  env
 * funcs: _putenv getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs Environment Variables (limit 32,767 characters per variable incl. NUL)
 * note:  PROBE: behaviour of _putenv for a "NAME=value" string of total length 32766 / 32767 / 32768 characters.
 *        Only the 32766 roundtrip is asserted. For 32767/32768 we assert internal consistency only
 *        (rejected: -1 and getenv NULL; accepted: 0) and print the outcome.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static char buf[40000];

/* total = strlen("TCENV_LONG=") + value length */
static void build(size_t total)
{
    size_t i, p = 11;
    memcpy(buf, "TCENV_LONG=", 11);
    for (i = p; i < total; i++) buf[i] = (char)('a' + (i % 26));
    buf[total] = 0;
}
static void boundary(const char *label, size_t total)
{
    int r; const char *g; size_t gl;
    build(total);
    r = _putenv(buf);
    g = getenv("TCENV_LONG");
    gl = g ? strlen(g) : 0;
    tc_write("TC NOTE " TC_TEST_ID "::"); tc_write(label); tc_write(r == 0 ? " accepted" : " rejected");
    tc_write(g ? " visible\n" : " invisible\n");
    if (r != 0) CHECK(g == NULL);
    else if (g) CHECK_EQ_U(gl, total - 11);
    _putenv("TCENV_LONG=");
}

TC_CASE(value_total_32766_accepted_roundtrip)
{
    const char *g;
    build(32766);
    CHECK_EQ_I(_putenv(buf), 0);
    g = getenv("TCENV_LONG");
    CHECK(g != NULL);
    if (g) { CHECK_EQ_U(strlen(g), 32766 - 11); CHECK(memcmp(g, buf + 11, 32766 - 11) == 0); }
    _putenv("TCENV_LONG=");
}
TC_CASE(boundary_32767) { boundary("32767", 32767); }
TC_CASE(boundary_32768_rejected_or_accepted_recorded) { boundary("32768", 32768); }

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(value_total_32766_accepted_roundtrip);
    TC_RUN(boundary_32767);
    TC_RUN(boundary_32768_rejected_or_accepted_recorded);
    return tc_end();
}
