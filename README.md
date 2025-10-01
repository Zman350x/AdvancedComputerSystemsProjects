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
frequency, I can set the governor to `performance` since that is what was
reccomended in Project 2. This was used for all testing.

SMT State: 2 threads per core

## General Info

Computer: Lenovo ThinkPad T16 Gen 2

Operating System: KDE neon User Edition (based on Ubuntu 24.04 LTS)

Linux Kernel Version: 6.14.0-32-generic (64-bit)

RAM: 32 GiB

Compiler used: g++ version 14.2
