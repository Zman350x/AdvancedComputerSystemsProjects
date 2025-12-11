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
            mean_cycles = mean(cycles))

print(ipc_mean)
