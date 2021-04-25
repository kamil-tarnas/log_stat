#!/bin/bash

# Usage:
# ./stat.sh <log_file> <stat1> <stat2> ... [--help] [--steps=<number>] [--height=<number>]

# Declare command line option arguments with default values
rawOpt=false
bucketsOpt=4 #TODO: Intelligent handling of the number of buckets - lazy evluation needed here
outOpt="/dev/null" # TODO: First argument to trace_echo...

# Match STAT(at least one space before value and possibly non-alphanumerical values)VALUE
ifsOpt="[[:space:]]*[^a-zA-Z0-9]*[[:space:]]+"

# Match VALUE - the default format for value for a given stat
valueFormatOpt="[0-9]+"


# TODO: Do not do any shifts? Just remove the element from the array?

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

  # bucketsOpt handling in case of "--bucketsOpt=" and "-b="
  -b=*|--buckets=*)
  bucketsOpt="${i#*=}"
  let numberOfOpts++
  ;;
  # bucketsOpt handling in case of "--bucketsOpt" and "-b", option provided - default value used
  -b|--buckets)
  bucketsOpt=4
  let numberOfOpts++
  ;;

  # ifsOpt handling in case of "--ifsOpt=" and "-i="
  -i=*|--ifs=*)
  ifsOpt="${i#*=}"
  let numberOfOpts++
  ;;
  # ifsOpt handling in case of "--ifsOpt" and "-i", option provided - default value used
  # TODO: Probaly not needed
  -i|--ifs)
  # TODO: Do not do anything for now
  # just increase the number of opts, to not be processed as a stat
  let numberOfOpts++
  ;;

  # valueFormatOpt handling in case of "--valueFormatOpt=" and "-v="
  -v=*|--value-format=*)
  valueFormatOpt="${i#*=}"
  let numberOfOpts++
  ;;
  # valueFormatOpt handling in case of "--valueFormatOpt" and "-v"
  # default value used #TODO: Probaly not needed
  -v|--value-format)
  # TODO: Do not do anything for now
  # just increase the number of opts, to not be processed as a stat
  let numberOfOpts++
  ;;

  *)
  # unknown option
  # TODO: If more options are provided - then add to ignored, to not be processes as a stat?
  ;;

esac
done

decomposedIterIncrement=3
logFile=$1; shift

numOfArgs=$#
let "numOfElemToSlice = numOfArgs - numberOfOpts"

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

# TODO: Main data structures (presently need to be unset after every iteration over a stat)
declare -A statOccurencesValueMap

declare -a bucketIntervals
declare -a bucketStatOccurrencesValueArray


# TODO: Make the script work on extracted logs to be more efficient
# logsExtracted=$(grep -E --color=never -o $statName$ifsOpt$valueFormatOpt $logFile)

# Desc: Calculates the total share of occurrences of a given stat
# Params: $1 the name of searched stat
# Return: The total number of occurrences for searched stat (regardless the value of stat)
calcTotalShare()
{
  declare statName=$1; shift
  totalOccurrences=$(grep -E --color=never -o $statName$ifsOpt$valueFormatOpt $logFile | wc -l)

  trace_echo $totalOccurrences

  echo $totalOccurrences
}


# Below is not used currently
calcTotalNumberOfValues()
{
  declare statName=$1; shift

  totalNumberOfValues=$(grep -E --color=never -o $statName$ifsOpt$valueFormatOpt $logFile \
    | sort -k 2 -n -t : | uniq -c | wc -l)

  trace_echo $totalNumberOfValues
}


# Desc: Prints the script output message after each processed stat
# Params: $1 the total number of occurrences of given stat
# Return: Nothing
printStatResults()
{
  declare totalOccurrences=$1; shift

  for stat in "${!statOccurencesValueMap[@]}" # TODO: Print results
  do
    value=${statOccurencesValueMap[$stat]}
    echo "Share of occurrences of $statName with value $($colorBlue)$stat$($colorReset): $($colorGreen)$(echo "scale=3;
          (100*$value/$totalOccurrences)" | bc -l | awk '{printf "%.3f\n", $0}')%$($colorReset)"
  done
  printf "\n"
}


calculateBuckets()
{
  let min=minValue
  let max=minValue+bucketIntervalWidth-1

  for ((bucket=0; (( $bucket < $bucketsOpt )) ; bucket++)); do

    # TODO: Bash does not support multi-dim arrays, need to store it in some other way...
    #bucketIntervals[$bucket]=(min, max)

    let min+=bucketIntervalWidth
    let max+=bucketIntervalWidth

    # TODO:Handling of bucket balancing
    # the last or first bucket needs to be bigger due to rounding
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

    trace_echo "Occurrences" $occurences
    trace_echo "Value:" $value

    #TODO: [[desc]] how many occurrences of specific value do we have?
    statOccurencesValueMap[$value]=$occurences
  done

  #TODO: If it is done then we can distribute the occurences over the bins
  #      using minValue and maxValue for calculating the intervals
  let bucketIntervalWidth=(maxValue-minValue)/bucketsOpt

  calculateBuckets
  #bucketIntervals
  #bucketStatOccurrencesValueArray

  #echo $bucketIntervalWidth #TODO: Value looks okay...
}


main()
{
  printf "\n"

  for statName in "${statNames[@]}"; do
    printf "Results for $($colorGreen)"$statName"$($colorReset):\n"

    totalOccurrences="$(calcTotalShare "$statName")"

    extractedStat=$(grep -E --color=never -o $statName$ifsOpt$valueFormatOpt $logFile \
      | sort -k 2 -n -t : | uniq -c)

    trace_echo "extractedStat is "$extractedStat

    read -a extractedStatDecomposed <<< $extractedStat

    trace_echo ${extractedStatDecomposed[@]}

    fillOccurrencesValueMap

    printStatResults "$totalOccurrences"

    # TODO: this in setting and unsetting
    # there is no holding on the stat values, everything is zeroed after every iteration
    unset statOccurencesValueMap
  done
}

# TODO: Make $1 as outOpt optional, so it will be overwritten if provided?
# Probably out-of-scope and will be left for the separate repository...
trace_echo()
{
  # Subtracting 2, one because of array starting with 0, the second one
  # because we do not want to have different behaviour when invoked with
  # "source" versus "./" and to be aligned with C++ implementation
  local numOfFunctionsOnStack=$(( ${#FUNCNAME[*]} - 2 ))
  echo -n "depth="$numOfFunctionsOnStack" " >> $outOpt

  # Print the call stack of current function (caller of trace_echo)
  for (( i=numOfFunctionsOnStack; i > 1; i-- )); do
    echo -n ${FUNCNAME[$i]}"()->" >> $outOpt
  done

  # Print the function name, which invoked trace_echo
  echo -n ${FUNCNAME[1]}"(): " >> $outOpt

  echo $@ >> $outOpt
}

trace_echo "cos"

# Start the script
main "$@"
