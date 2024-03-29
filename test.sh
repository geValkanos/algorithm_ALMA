# Correct input format
# bash test.sh my_program input_Folder

# Set the timeout in seconds
TIMEOUT=1

RED='\033[031m'
GREEN='\033[032m'
ORANGE='\033[033m'
CYAN='\033[036m'
NC='\033[0m'

# Check if the number of arguments is correct.
if [ $# != 3 ]
then
  # First is the executabele.
  # Second is the folder.
  if [ -x $1 ] && [ -d $2 ]
  then
      input=$(find $2 | grep input*)
      #Iterate the input files.
      for testcase in $input
      do
        correct_output=$2'output'${testcase##*input}
        runExecutable=$(timeout $TIMEOUT './'$1 < $testcase > out)
        if [ $? -eq 124 ]
        then
          echo -e "${RED}TIMEOUT${NC}: ${ORANGE}${testcase}${NC} Execution took more than 1 second"
        else
          diff -q out $correct_output > /dev/null
          if [ ! $status ]
          then
            echo -e "[${GREEN}Correct${NC}]" " ${CYAN}${testcase}${NC}:" 
          else
            echo -e "[${RED}Wrong${NC}]" " ${ORANGE}${testcase}${NC}"
          fi
        fi
	rm out
      done
  else
    echo "Error: First argument must be executable, Second must be a folder"
  fi
else
  echo "Error: Wrong number of arguments"
fi
