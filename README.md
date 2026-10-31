# msvcrt / ucrtbase test corpus

About 6,000 small C tests for the Microsoft C runtimes (ucrtbase, msvcr120/110/100/90/80, msvcrt.dll), built with
mingw-w64 and meant to be run on **real Windows**. Wine is only a smoke runner here: a test failing on Wine is not wrong
for that reason alone. The test format and the source-of-truth rules are in [SCHEMA.md](SCHEMA.md).

```
python3 tools/tc.py lint
python3 tools/tc.py build [FILTER] [--crt ucrtbase]
python3 tools/tc.py run   [FILTER]            # Wine smoke run; needs WINEPREFIX (use ./.pfx) and the caps below
python3 tools/tc.py ingest DIR                # summarise results copied back from a real Windows run
```

`tools/run_on_windows.ps1` runs the built tree on Windows. `build/`, `.pfx/` and every Wine prefix file are generated and
git-ignored; rebuild with `tc.py build`.

## Memory hazards: read this before running anything

An early unattended run of this corpus under Wine exhausted the host's 128 GB and took the machine (and its X session)
down. Huge-allocation negative tests are the main suspects: Windows fails a 64 TB request instantly, but Wine stalls on
it, and nothing bounded the damage. Two guards now exist. Use both.

1. **Per-test RAM cap in `tools/tc.py`.** Each test runs in its own `systemd-run --user --scope` with
   `MemoryMax=${TC_MEMMAX:-4G}` and `MemorySwapMax=0`. A runaway test is OOM-killed and reported as `CRASH`.
   On timeout (`TC_TIMEOUT`, default 60 s) the whole scope is SIGKILLed, so Wine children cannot linger.
   `TC_MEMMAX=0` disables the cap (do not, on a machine you care about). This needs a running `systemd --user`.
2. **Hazard cases are compiled out by default.** A case that starts with `TC_HAZARD("reason")` is skipped
   (`TC SKIP ... hazard, build with TC_HAZARD=1`) unless the corpus is built with `TC_HAZARD=1 python3 tools/tc.py build`.
   Build with it on real Windows, where those requests fail fast. Current hazard cases (all a 1<<46 = 64 TB
   `_aligned_malloc`):
   - `tests/heap/hp_neg_aligned_malloc_huge.c` (`one_shl_46`; a `1<<42` case now runs by default)
   - `tests/ucrt_only/ucr_neg_aligned_malloc_huge.c` (`size_16tib`)
   - `tests/ucrt_only/ucr_neg_aligned_malloc_huge_errno_probe.c` (`errno_16tib`, `iph_count`)

**Never add `MemoryHigh`.** With swap off it throttles a process instead of killing it. That stall shows up as memory
pressure on `user@UID.service`, and `systemd-oomd` (50 % pressure for 20 s) then kills the whole user manager, which
takes down dbus and the desktop session. This happened once during testing. Use `MemoryMax` alone.

When you add a test that requests a very large size, run it under the cap first, and wrap any case that stalls or
allocates in `TC_HAZARD(...)`.

## Status (honest)

- 5,986 tests, lint clean; 28,330 of 28,359 per-CRT builds succeed.
  Not building (no exe, so not run): `gmc_blank_basic`, `gmc_blank_ideographic_probe`, `mth_neg_pow_zero_negexp_value`,
  `mth_neg_pow_zero_neginf_probe`, `tm_neg_asctime_invalid_fields`, `tm_neg_asctime_s_fields` (compile errors on some
  CRTs) and `ct_isascii_exported_probe` (not imported by any target).
- Never validated on real Windows. Many tests are `probe` (about 2,300) or `inferred` (100): their expected values are
  reasoned, not observed. Treat Windows results as the source of truth, and fix tests, not the reverse.
- Wine smoke runs show many failures that need triage into test bugs and genuine Wine differences. Wine does not
  implement the `_aligned_*recalloc` family, so those tests CRASH there by design. Results files under `results/` are
  from interrupted runs and are not a baseline.
- Function coverage: 865 of 1,213 exported functions; 348 are uncovered (gap fill was cut short by an agent cap).
