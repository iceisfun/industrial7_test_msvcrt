/* TC
 * id:    fio_neg_probe_fopen_name_too_long
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: 
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen; Win32 ERROR_FILENAME_EXCED_RANGE
 * note:  PROBE: fopen of a single path component of 256, 300 and 40000 chars (all exceed NTFS 255-char component limit) must return NULL without crashing; errno expected in {ENOENT, EINVAL, ENAMETOOLONG} (guess ENOENT).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define V TC_S1(TC_CRT_VER)
static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
static char *mk(size_t n) { char *p = (char *)HeapAlloc(GetProcessHeap(), 0, n + 1); size_t i; if (!p) return NULL; for (i = 0; i < n; i++) p[i] = 'a'; p[n] = 0; return p; }
static void probe(size_t n)
{
    char *nm = mk(n); FILE *f; int e;
    CHECK(nm != NULL); if (!nm) return;
    errno = 0; f = fopen(nm, "w"); e = errno;
    CHECK(f == NULL);
    CHECK(e == ENOENT || e == EINVAL || e == ENAMETOOLONG);
    if (f) { fclose(f); DeleteFileA(nm); }
    HeapFree(GetProcessHeap(), 0, nm);
}
TC_CASE(len_256) { probe(256); }
TC_CASE(len_300) { probe(300); }
TC_CASE(len_40000) { probe(40000); }
int main(void)
{
    tc_begin();
    if (TC_AT_LEAST(80)) tc_iph_install();
    TC_RUN(len_256); TC_RUN(len_300); TC_RUN(len_40000);
    return tc_end();
}
