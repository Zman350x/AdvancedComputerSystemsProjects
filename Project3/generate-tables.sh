#!/bin/env bash

# GENERATED WITH HELP FROM CHATGPT

awk -F',' '
BEGIN {
    printf "%-20s %-20s %-20s %-20s %-20s %-20s %-20s\n", "Workload", "Read Avg (us)", "Read P95 (us)", "Read P99 (us)", "Write Avg (us)", "Write P95 (us)", "Write P99 (us)";
}
FNR>1 {
    read_avg=$16;   # read_clat_mean_us
    write_avg=$57;  # write_clat_mean_us
    read_p95=""; read_p99=""; write_p95=""; write_p99="";
    p95_count=0; p99_count=0;

    for(i=1;i<=NF;i++){
        if($i ~ /95\.?0*%=/){
            split($i,a,"=");
            if(p95_count==0) read_p95=a[2];
            else if(p95_count==1) write_p95=a[2];
            p95_count++;
        }
        if($i ~ /99\.?0*%=/){
            split($i,a,"=");
            if(p99_count==0) read_p99=a[2];
            else if(p99_count==1) write_p99=a[2];
            p99_count++;
        }
    }

    printf "%-20s %-20s %-20s %-20s %-20s %-20s %-20s\n", $3, read_avg, read_p95, read_p99, write_avg, write_p95, write_p99;
}' csvs/rand4k_qd1_result.csv csvs/seq128k_qd1_result.csv
