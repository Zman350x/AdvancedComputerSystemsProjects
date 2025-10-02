#!/bin/env Rscript

# GENERATED IN PART WITH CHATGPT

library(tidyverse)

# --- 1. Read CSV ---
df <- read_csv("csvs/bs_sweep_rand_result.csv")

# Extract block size from jobname (assumes ending with 4k, 8k, 128k, etc.)
df <- df %>%
  mutate(block_size = sub(".*?(\\d+k)$", "\\1", jobname)) %>%
  arrange(as.numeric(sub("k", "", block_size)))

# Convert numeric columns
df <- df %>%
  mutate(read_iops = as.numeric(read_iops),
         read_bandwidth_mb = as.numeric(read_bandwidth_kb) / 1024,
         block_size_kb = as.numeric(sub("k", "", block_size)))

# Scale MB/s independently relative to IOPS
mb_max <- max(df$read_bandwidth_mb, na.rm = TRUE)
iops_max <- max(df$read_iops, na.rm = TRUE)
scale_factor <- iops_max / mb_max

ggplot(df, aes(x = block_size_kb)) +
  geom_line(aes(y = read_iops, color = "IOPS", group = 1), linewidth = 1) +
  geom_point(aes(y = read_iops, color = "IOPS"), size = 2) +
  geom_line(aes(y = read_bandwidth_mb * scale_factor, color = "MB/s", group = 1), linewidth = 1) +
  geom_point(aes(y = read_bandwidth_mb * scale_factor, color = "MB/s"), size = 2) +
  scale_y_continuous(
    name = "IOPS",
    sec.axis = sec_axis(~. / scale_factor, name = "MB/s")
  ) +
  scale_x_continuous(
    name = "Block Size (KB)",
    breaks = df$block_size_kb
  ) +
  labs(color = "Metric", title = "FIO Read Performance") +
  theme_classic()
