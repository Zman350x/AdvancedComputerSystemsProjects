#!/bin/env Rscript

library(tidyverse)

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
  geom_jitter(width = 0.25, height = 0, colour = "blue", shape = 21) +
  labs(x = "Program", y = "Duration (s)", title = "IPC Duration by Program Type")

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
  geom_jitter(width = 0.25, height = 0, colour = "blue", shape = 21) +
  labs(x = "Group", y = "Duration (s)", title = "Fixed Task Duration by CPU Control Croup")

ggplot(data = filter(cgroups, cgroup != "baseline"), aes(x = cgroup, y = duration)) +
  geom_boxplot() +
  geom_jitter(width = 0.25, height = 0, colour = "blue", shape = 21) +
  labs(x = "Group", y = "Duration (s)", title = "Fixed Task Duration by CPU Control Croup", subtitle = "baseline excluded")
