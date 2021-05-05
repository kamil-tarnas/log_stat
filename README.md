# log_stat
log_stat is a Bash shell script or C++ program for statistical analysis of data from log text files.

## Usage

log_stat can calculate the share of occurrences of specific value of parameter provided by the user. There are two available versions of this program, one in a form of Bash shell script the other one is C++ compiled application.

## Bash

Usage:
```bash
./log_stat.sh <log_file> <parameter_1> <parameter_2> ... [--raw] [--out=<output_file_or_stream>]
```

For example:
```bash
./log_stat.sh log_02_03_2021 user errorCode --raw
```
Will search in log_02_03_2021 file for the number of occurrences of the values of 'user' parameter and the values of 'errorCode' parameter and calculate the shares of each value of parameter then print the results in a raw format (without using text coloring).


## C++

Build:
```bash
make
```

Usage:
```bash
./log_stat.out log_02_03_2021 user errorCode --raw
```

Will search in log_02_03_2021 file for the number of occurrences of the values of 'user' parameter and the values of 'errorCode' parameter and calculate the shares of each value of parameter then print the results in a raw format (without using text coloring).