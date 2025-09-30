# Advanced Computer Systems Projects

## Repository Layout

Each of the three projects can be found in their own folders (`ProjectX`), and
all relevant can be found within. Compiler flags and options can be found in the
`Makefile` within each project folder, as can a `project-report.md` file which
contains the report for the project.

## CPU Info

Model: Intel i7-1355U

Vector Width: AVX, so 256-bit

Cache sizes:
* L1d:  352 KiB (10 instances)
* L1i:  576 KiB (10 instances)
* L2:   6.5 MiB (4 instances)
* L3:   12 MiB (1 instance)

My laptop does not support userspace CPU governor, so while I can't fix the
frequency, I can set the governor to `powersave` since I figured that if I
capped the CPU at a lower frequency, it was more likely to run at that limit
than if I let it run full-speed, in which case it may throttle/slow down.

SMT State: 2 threads per core
