# log_stat
log_stat is a Bash shell script for statistical analysis of data from log text files.

## Usage
log_stat can calculate the share of occurrences of specific value of parameter provided by the user.

Usage:
```bash
./log_stat.sh <log_file> <parameter_1> <parameter_2> ... [--raw] [--out=<output_file_or_stream>]
```

For example:
```bash
./distribute_work.sh log_02_03_2021 user errorCode --raw
```
Will search in log_02_03_2021 file for the number of occurrences of the values of 'user' parameter and the values of 'errorCode' parameter and calculate the shares of each value of parameter then print the results in a raw format (without using text coloring).
