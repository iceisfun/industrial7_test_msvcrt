/* TC
 * id:    hp_malloc_fill_sizes
 * area:  heap
 * funcs: malloc free
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3.4

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

TC_CASE(boundaries)
{
    static const size_t base[] = {1016, 1024, 1032, 4088, 4096, 65536, 524288, 1048576, 4194304};
    size_t i; int d;
    for (i = 0; i < sizeof base / sizeof base[0]; i++)
        for (d = -8; d <= 8; d += 8) {
            size_t n = base[i] + d; unsigned char *p; vs = n; p = (unsigned char *)malloc(vs);
            CHECK(p != NULL); if (!p) continue;
            p[0] = 0xA5; p[n - 1] = 0x5A; p[n / 2] = 0x3C;
            CHECK(p[0] == 0xA5 && p[n - 1] == 0x5A && p[n / 2] == 0x3C);
            free(p);
        }
}
TC_CASE(full_pattern)
{
    static const size_t n[] = {1023, 1025, 4095, 4097, 65537};
    size_t i;
    for (i = 0; i < 5; i++) { unsigned char *p; vs = n[i]; p = (unsigned char *)malloc(vs); CHECK(p != NULL); if (p) { fillp(p, n[i], 9); CHECK(chkp(p, n[i], 9)); } free(p); }
}
int main(void) { tc_begin(); TC_RUN(boundaries); TC_RUN(full_pattern); return tc_end(); }
