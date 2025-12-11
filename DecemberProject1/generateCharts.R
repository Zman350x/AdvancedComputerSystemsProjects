#!/bin/env Rscript

library(tidyverse)

# IPC

ipc <- read.csv("csvs/ipc.csv")

ipc$start_time <- as.POSIXct(ipc$start_time, format = "%Y-%m-%d %H:%M:%OS", tz = "UTC")
ipc$end_time <- as.POSIXct(ipc$end_time, format = "%Y-%m-%d %H:%M:%OS", tz = "UTC")
ipc$duration <- as.numeric(ipc$end_time - ipc$start_time)
ipc$cycles <- as.numeric(ipc$end_cycles - ipc$start_cycles)

ipc_mean <- ipc %>%
  group_by(program) %>%
  summarize(mean_duration = mean(duration),
            min_duration = min(duration),
            max_duration = max(duration),
            sd_duration = sd(duration),
            mean_cycles = mean(cycles),
            min_cycles = min(cycles),
            max_cycles = max(cycles),
            sd_cycles = sd(cycles))

print(ipc_mean, n = Inf, width = Inf)

ggplot(data = ipc, aes(x = program, y = duration)) +
  geom_boxplot() +
  labs(x = "Program", y = "Duration (s)", title = "IPC Duration by Program Type")

# CGROUPS

cgroups <- read.csv("csvs/cgroups.csv")

cgroups_mean <- cgroups %>%
  group_by(cgroup) %>%
  summarize(mean_duration = mean(duration),
            min_duration = min(duration),
            max_duration = max(duration),
            sd_duration = sd(duration),
            mean_cycles = mean(cycles),
            min_cycles = min(cycles),
            max_cycles = max(cycles),
            sd_cycles = sd(cycles))

print(cgroups_mean, n = Inf, width = Inf)

ggplot(data = cgroups, aes(x = cgroup, y = duration)) +
  geom_boxplot() +
  labs(x = "Group", y = "Duration (s)", title = "Fixed Task Duration by CPU Control Croup")

ggplot(data = filter(cgroups, cgroup != "baseline"), aes(x = cgroup, y = duration)) +
  geom_boxplot() +
  labs(x = "Group", y = "Duration (s)", title = "Fixed Task Duration by CPU Control Croup", subtitle = "baseline excluded")

# THP

thp <- read.csv("csvs/thp.csv")

thp_mean <- thp %>%
  group_by(is_thp) %>%
  summarize(mean_duration = mean(duration),
            min_duration = min(duration),
            max_duration = max(duration),
            sd_duration = sd(duration),
            mean_cycles = mean(cycles),
            min_cycles = min(cycles),
            max_cycles = max(cycles),
            sd_cycles = sd(cycles))

print(thp_mean, n = Inf, width = Inf)

ggplot(data = thp, aes(x = is_thp, y = duration)) +
  geom_boxplot() +
  labs(x = "THP Status", y = "Duration (s)", title = "Memory Access Benchmark with and without THP")
