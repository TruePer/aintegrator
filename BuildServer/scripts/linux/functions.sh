# Colors
red='\e[0;31m'
green='\e[0;32m'
blue='\e[0;34m'
NC='\e[0m' # No Color

# Functions ----------------------------------------------------------------------------------------
check_error()
{
  # ${1} - return code
  # ${2} - details message
  if [ "${1}" -ne "0" ]; then
  	echo -e "${red}ERROR: ${blue}${2}${NC}"
    exit ${1}
  fi
}

message()
{
	# $1 - message
	# $2 - details
	
	echo ""
	echo -e "$green-- $1 $blue $2"
	echo -e "$green-------------------------------------------------$NC"
}

