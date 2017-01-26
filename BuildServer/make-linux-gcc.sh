#!/bin/bash

# Current directory --------------------------------------------------------------------------------
BaseDir=`dirname $0`
CurrentPath=`cd $BaseDir; pwd`
SourcePath=${CurrentPath}/..

pushd ${SourcePath}
ProjectDirName=`basename "$PWD"`
popd

BuildDir=${SourcePath}/../${ProjectDirName}-build

source ${SourcePath}/BuildServer/scripts/linux/functions.sh
source ${SourcePath}/BuildServer/scripts/linux/make-deb.sh

# Set Qt path -------------------------------------------------------------------------------------
QtPathFile=$SourcePath/../qt-env.sh

if [ -f $QtPathFile ]; then
	source $QtPathFile
	echo "QtPathFile: $QtPathFile"
	echo "QTDIR=$QTDIR"
	echo "PATH=$PATH"
fi

# Build 3Gpp project -------------------------------------------------------------------------------

message "BUILD STARTING..."

mkdir ${BuildDir}
pushd ${BuildDir}

message "CMAKE" #-----------------------------------------------------------------------------------

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ${SourcePath}
check_error $? "cmake failed"
return 0
message "BUILD" #-----------------------------------------------------------------------------------

ProcessorsCount=`cat /proc/cpuinfo | grep processor | wc -l`

message "Processors count:" ${ProcessorsCount}

make -j${ProcessorsCount} all install
check_error $? "build has errors"

popd

message "MAKE DEB-PACKAGE" #------------------------------------------------------------------------
make_deb ${SourcePath}/BuildServer

message "BUILDING WAS FINISHED"
