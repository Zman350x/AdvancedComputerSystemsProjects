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
  geom_vline(xintercept = 64, linetype = "dashed", color = "darkgrey") +
  scale_y_continuous(
    name = "IOPS",
    sec.axis = sec_axis(~. / scale_factor, name = "MB/s")
  ) +
  scale_x_continuous(
    name = "Block Size (KB)",
    breaks = df$block_size_kb
  ) +
  labs(color = "Metric", title = "Random Read Performance") +
  theme_classic()

# --- 2. Plot average read latency ---
df <- df %>%
  mutate(read_avg_lat_us = as.numeric(read_clat_mean_us))

ggplot(df, aes(x = block_size_kb, y = read_avg_lat_us)) +
  geom_line(aes(group = 1), linewidth = 1, color = "darkgreen") +
  geom_point(size = 2, color = "darkgreen") +
  geom_vline(xintercept = 64, linetype = "dashed", color = "darkgrey") +
  scale_x_continuous(
    name = "Block Size (KB)",
    breaks = df$block_size_kb,
  ) +
  scale_y_continuous(name = "Average Read Latency (us)") +
  labs(title = "Average Random Read Latency vs Block Size") +
  theme_classic()

# --- 1. Read CSV ---
df <- read_csv("csvs/bs_sweep_seq_result.csv")

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
  geom_vline(xintercept = 64, linetype = "dashed", color = "darkgrey") +
  scale_y_continuous(
    name = "IOPS",
    sec.axis = sec_axis(~. / scale_factor, name = "MB/s")
  ) +
  scale_x_continuous(
    name = "Block Size (KB)",
    breaks = df$block_size_kb
  ) +
  labs(color = "Metric", title = "Sequential Read Performance") +
  theme_classic()

# --- 2. Plot average read latency ---
df <- df %>%
  mutate(read_avg_lat_us = as.numeric(read_clat_mean_us))

ggplot(df, aes(x = block_size_kb, y = read_avg_lat_us)) +
  geom_line(aes(group = 1), linewidth = 1, color = "darkgreen") +
  geom_point(size = 2, color = "darkgreen") +
  geom_vline(xintercept = 64, linetype = "dashed", color = "darkgrey") +
  scale_x_continuous(
    name = "Block Size (KB)",
    breaks = df$block_size_kb,
  ) +
  scale_y_continuous(name = "Average Read Latency (us)") +
  labs(title = "Average Sequential Read Latency vs Block Size") +
  theme_classic()


# --- 1. Read CSV ---
df <- read_csv("csvs/rand_mix_sweep_result.csv")

x_labels <- c(
  "rand3070" = "30% Read / 70% Write",
  "rand5050" = "50% Read / 50% Write",
  "rand7030" = "70% Read / 30% Write",
  "randread" = "100% Read",
  "randwrite" = "100% Write"
)

# Convert numeric columns
df <- df %>%
  mutate(
    read_iops = as.numeric(read_iops),
    read_mb_s = as.numeric(read_bandwidth_kb) / 1024,
    write_iops = as.numeric(write_iops),
    write_mb_s = as.numeric(write_bandwidth_kb) / 1024
  )

# --- 2. Determine scaling factor for secondary axis ---
iops_max <- max(c(df$read_iops, df$write_iops), na.rm = TRUE)
mb_max <- max(c(df$read_mb_s, df$write_mb_s), na.rm = TRUE)
scale_factor <- iops_max / mb_max

# --- 3. Reshape to long format for bars (Read + Write IOPS only) ---
df_long <- df %>%
  select(jobname, read_iops, write_iops) %>%
  pivot_longer(cols = c(read_iops, write_iops),
               names_to = "metric",
               values_to = "value") %>%
  mutate(metric_label = ifelse(metric == "read_iops", "Read", "Write"))

# --- 4. Plot bars ---
ggplot(df_long, aes(x = jobname, y = value, fill = metric_label)) +
  geom_bar(stat = "identity", position = position_dodge(width = 0.7), width = 0.6) +
  scale_y_continuous(
    name = "IOPS",
    sec.axis = sec_axis(~. / scale_factor, name = "MB/s")  # secondary axis for reference
  ) +
  scale_x_discrete(labels = x_labels) +  # <-- rename X labels
  labs(x = "Read/Write Ratio", fill = "Metric",
       title = "Performance vs Read/Write Ratio") +
  theme_classic() +
  theme(
    axis.text.x = element_text(angle = 45, hjust = 1),
  )

df_long_lat <- df %>%
  select(jobname, read_clat_mean_us, write_clat_mean_us) %>%
  pivot_longer(cols = c(read_clat_mean_us, write_clat_mean_us),
               names_to = "metric",
               values_to = "lat_us") %>%
  mutate(metric_label = ifelse(metric == "read_clat_mean_us", "Read", "Write"))

# --- 6. Plot average latency bars ---
ggplot(df_long_lat, aes(x = jobname, y = lat_us, fill = metric_label)) +
  geom_bar(stat = "identity", position = position_dodge(width = 0.7), width = 0.6) +
  scale_x_discrete(labels = x_labels) +
  labs(x = "Read/Write Ratio", y = "Average Latency (us)", fill = "Metric",
       title = "Average Read/Write Latency vs Read/Write Ratio") +
  theme_classic() +
  theme(
    axis.text.x = element_text(angle = 45, hjust = 1)
  )
