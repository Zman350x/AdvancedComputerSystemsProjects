#!/bin/env Rscript

library(tidyverse)
library(knitr)

# TREE

tree <- read.csv("csvs/tree.csv")

tree_mean <- tree %>%
  group_by(locking, job, threads, keys) %>%
  summarize(mean_duration = mean(duration),
            min_duration = min(duration),
            max_duration = max(duration),
            sd_duration = sd(duration),
            mean_cycles = mean(cycles),
            min_cycles = min(cycles),
            max_cycles = max(cycles),
            sd_cycles = sd(cycles))

kable(tree_mean, format = "markdown", align = "c")

tree$labels <- factor(paste(tree$threads, "thread", tree$locking),
                      levels = c("1 thread coarse",
                                 "2 thread coarse",
                                 "4 thread coarse",
                                 "8 thread coarse",
                                 "12 thread coarse",
                                 "1 thread read_write",
                                 "2 thread read_write",
                                 "4 thread read_write",
                                 "8 thread read_write",
                                 "12 thread read_write"))

tree_readonly_millionkeys <- tree %>% filter(job == "read_only", keys == 1000000)
tree_writeonly_millionkeys <- tree %>% filter(job == "write_only", keys == 1000000)
tree_readwrite_millionkeys <- tree %>% filter(job == "read_write", keys == 1000000)
tree_readonly_hundredthoukeys <- tree %>% filter(job == "read_only", keys == 100000)
tree_writeonly_hundredthoukeys <- tree %>% filter(job == "write_only", keys == 100000)
tree_readwrite_hundredthoukeys <- tree %>% filter(job == "read_write", keys == 100000)
tree_readonly_tenthoukeys <- tree %>% filter(job == "read_only", keys == 10000)
tree_writeonly_tenthoukeys <- tree %>% filter(job == "write_only", keys == 10000)
tree_readwrite_tenthoukeys <- tree %>% filter(job == "read_write", keys == 10000)

ggplot(data = tree_readonly_millionkeys, aes(x = labels, y = duration, group = labels)) +
  geom_boxplot() +
  labs(x = "Threads and Locking", y = "Duration (s)", title = "Read-only 1mil key performance by locking type and thread count") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1))

ggplot(data = tree_writeonly_millionkeys, aes(x = labels, y = duration, group = labels)) +
  geom_boxplot() +
  labs(x = "Threads and Locking", y = "Duration (s)", title = "Write-only 1mil key performance by locking type and thread count") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1))

ggplot(data = tree_readwrite_millionkeys, aes(x = labels, y = duration, group = labels)) +
  geom_boxplot() +
  labs(x = "Threads and Locking", y = "Duration (s)", title = "70/30 read/write 1mil key performance by locking type and thread count") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1))

ggplot(data = tree_readonly_hundredthoukeys, aes(x = labels, y = duration, group = labels)) +
  geom_boxplot() +
  labs(x = "Threads and Locking", y = "Duration (s)", title = "Read-only 100k key performance by locking type and thread count") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1))

ggplot(data = tree_writeonly_hundredthoukeys, aes(x = labels, y = duration, group = labels)) +
  geom_boxplot() +
  labs(x = "Threads and Locking", y = "Duration (s)", title = "Write-only 100k key performance by locking type and thread count") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1))

ggplot(data = tree_readwrite_hundredthoukeys, aes(x = labels, y = duration, group = labels)) +
  geom_boxplot() +
  labs(x = "Threads and Locking", y = "Duration (s)", title = "70/30 read/write 100k key performance by locking type and thread count") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1))

ggplot(data = tree_readonly_tenthoukeys, aes(x = labels, y = duration, group = labels)) +
  geom_boxplot() +
  labs(x = "Threads and Locking", y = "Duration (s)", title = "Read-only 10k key performance by locking type and thread count") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1))

ggplot(data = tree_writeonly_tenthoukeys, aes(x = labels, y = duration, group = labels)) +
  geom_boxplot() +
  labs(x = "Threads and Locking", y = "Duration (s)", title = "Write-only 10k key performance by locking type and thread count") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1))

ggplot(data = tree_readwrite_tenthoukeys, aes(x = labels, y = duration, group = labels)) +
  geom_boxplot() +
  labs(x = "Threads and Locking", y = "Duration (s)", title = "70/30 read/write 10k key performance by locking type and thread count") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1))
