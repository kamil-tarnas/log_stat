#!/bin/bash


# usage:
# ./stat.sh <log_file> <stat1> <stat2> ... [--help] [--steps=<number>] [--height=<number>]

# MVP_DONE:: Multiple stats to be processed
# TODO: Prevent multiple passes over files
# TODO: Read into an variable (preserving the \n s and so on) to avoid multiple greps
# TODO: Do the calculations in intervals - use percents or linear scale or logarithmic scale
# TODO: Make it show nice charts - visualize the data usig Python script
# TODO: Make .* be more clever to not match statName something...
# REFINE: bc needs to print leading zeros
# MVP_DONE: Printing one leading zero - maybe this is even better for readability sake?
# TODO: Make a custom flag for fractional part precision in percentage printing (the scale=3; and %.3f\n)
# TODO: Make the statictics printing aligned - that is the output values should be in one column or
#       something like that

# TODO: [[big]] Parametrize everything - the output of echos, the structure (that is 1. number of occur, 2. stat name, 3. stat value
#               although this might not be needed), support non-numeric values of stats, make a stat-value separator custom (something
#               lke -F or -IFS flag (supposedly a regex?), make the "iter+=3" (the number 3 in that expression) custom (this might not be
#               needed - it depends on the approach taken
#       part1: --buckets=x (for example '--buckets=10') distribute the values over equally-sized buckets
#              of the number of x (need to take a maxmimal
#       part2: --linear-buckets=x and --log-buckets=x

# MVP_DONE: Colorful percentages and stat values - if it won't break anything - setting and unsetting with --raw flag
#           is in progress
# TODO: Bind the trace echo utility...
# TODO: Non-numerical values of stat
# TODO: Enable log redirect --log=none (is default), --log=stdout --log=stderr
# TODO: Modularize to a config part, run part and so on...


# Declare command line option arguments with default values
rawOpt=false
numberOfBuckets=4 # TODO: Intelligent handling of the number of buckets - lazy evalueation needed here
outOpt="/dev/null" # TODO: First argument to trace_echo...

# TODO: make it support both --raw, --raw=true and --raw=false
# TODO: need to exclude opts from the calculations of stats
# TODO: Do not do any shifts? Just remove the element from the array?
#for i in "${!array[@]}"; do
#${array[i]}
  #for i in "${!@}" - works

numberOfOpts=0

for i in "$@"
do
case $i in
  # rawOpt handling in case of "--opt=" and "-o="
  -r=*|--raw=*)
  rawOpt="${i#*=}"
  let numberOfOpts++
  ;;
  # rawOpt handling in case of "--opt" and "-o", option provided - default value used
  -r|--raw)
  rawOpt=true
  let numberOfOpts++
  ;;

  # outOpt handling in case of "--opt=" and "-o="
  -o=*|--out=*)
  outOpt="${i#*=}"
  let numberOfOpts++
  ;;
  # outOpt handling in case of "--opt" and "-o", option provided - default value used
  -o|--out)
  outOpt="log"
  let numberOfOpts++
  ;;
  *)
  # unknown option
  ;;
esac
done
#echo "rawOpt  = ${rawOpt}"
#echo $numberOfOpts

decomposedIterIncrement=3
logFile=$1; shift

numOfArgs=$#
let "numOfElemToSlice = numOfArgs - numberOfOpts"
#echo $numOfElemToSlice

declare -a statNames=("$@") # Put it here if it is not a flag...
statNames=("${statNames[@]::numOfElemToSlice}")

# TODO: Move to some program constant segment
# TODO: If with --raw flag then unset those values
colorGreen="tput setaf 2"
colorBlue="tput setaf 4"
colorReset="tput sgr0"

if [ "$rawOpt" = true ] ; then
  unset colorGreen colorBlue colorReset
  #unset $(compgen -v | grep "color.*")
fi

#TODO: Main data structures (presently need to be unset after every iteration over a stat)
declare -A statOccurencesValueMap

declare -a bucketIntervals
declare -a bucketStatOccurrencesValueArray

# TODO: Make the script work on extracted logs to be more efficient
#logsExtracted=$(grep -E --color=never -o $statName.*[0-9]+ $logFile)

calcTotalShare()
{
  declare statName=$1; shift
  totalOccurrences=$(grep -E --color=never -o $statName.*[0-9]+ $logFile | wc -l)
  echo $totalOccurrences >> $outOpt
}


# Below is not used currently
calcTotalNumberOfValues()
{
  declare statName=$1; shift
  totalNumberOfValues=$(grep -E --color=never -o $statName.*[0-9]+ $logFile | sort -k 2 -n -t : | uniq -c | wc -l)
  echo $totalNumberOfValues >> $outOpt
}


printStatResults()
{
  for stat in "${!statOccurencesValueMap[@]}" # TODO: Print results
  do
    value=${statOccurencesValueMap[$stat]}
    echo "Share of occurrences of $statName with value $($colorBlue)$stat$($colorReset): $($colorGreen)$(echo "scale=3;
          (100*$value/$totalOccurrences)" | bc -l | awk '{printf "%.3f\n", $0}')%$($colorReset)"
  done
}


calculateBuckets()
{
  let min=minValue
  let max=minValue+bucketIntervalWidth-1

  for ((bucket=0; (( $bucket < $numberOfBuckets )) ; bucket++)); do

    # TODO: Bash does not support multi-dim arrays, need to store it in some other way...
    #bucketIntervals[$bucket]=(min, max)

    let min+=bucketIntervalWidth
    let max+=bucketIntervalWidth

    #TODO:Handling of bucket balancing - the last or first bucket needs to be bigger due to rounding
  done
}


fillOccurrencesValueMap()
{
  numOfStatDecomposedElem=${#extractedStatDecomposed[*]}

  #TODO: Will be used for bucket calculations
  declare minValue
  declare maxValue

  for ((iter=0; (( $iter < $numOfStatDecomposedElem )) ; iter+=decomposedIterIncrement)); do
    occurences=${extractedStatDecomposed[$iter]}
    value=${extractedStatDecomposed[$((iter+2))]}

    if [[ $value -lt $minValue ]]; then
      minValue=$value
    fi

    if [[ $value -gt $maxValue ]]; then
      maxValue=$value
    fi

    echo "Occurrences" $occurences >> $outOpt
    echo "Value:" $value >> $outOpt
    statOccurencesValueMap[$value]=$occurences #TODO: [[desc]] how many occurrences of specific value do we have?
  done

  #TODO: If it is done then we can distribute the occurences over the bins using minValue and maxValue
  #      for calculating the intervals
  let bucketIntervalWidth=(maxValue-minValue)/numberOfBuckets

  calculateBuckets
  #bucketIntervals
  # bucketStatOccurrencesValueArray

  #echo $bucketIntervalWidth #TODO: Value looks okay...
}


for statName in "${statNames[@]}"; do
  printf "\nResults for $($colorGreen)"$statName"$($colorReset):\n"

  calcTotalShare "$statName"

  extractedStat=$(grep -E --color=never -o $statName.*[0-9]+ $logFile | sort -k 2 -n -t : | uniq -c)
  echo "extractedStat is"$extractedStat >> $outOpt

  read -a extractedStatDecomposed <<< $extractedStat
  echo ${extractedStatDecomposed[@]} >> $outOpt

  fillOccurrencesValueMap

  printStatResults

  unset statOccurencesValueMap #TODO: this in setting and unsetting - there is no holding on the stat values, everything is zeroed after every iteration
done
