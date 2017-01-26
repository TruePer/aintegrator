#!/bin/bash

# Current directory
BASEDIR=`dirname $0`
CURRENT_PATH=`cd $BASEDIR; pwd`

# Externals
export LD_LIBRARY_PATH=$CURRENT_PATH/../bin:$LD_LIBRARY_PATH

# Directories
PROTO_PATH=$CURRENT_PATH/../proto
CPP_PATH=$CURRENT_PATH/../code/proto

# Protobuf generator
PROTOC=$CURRENT_PATH/../bin/protoc

rm -rf $CPP_PATH
mkdir -p $CPP_PATH

# Generate sources
echo "Generating from proto files..."

for file in $(find $PROTO_PATH -name '*.proto')
do 
  echo $(basename $file)
  PROTO_DIR_PATH=$(dirname $file)
  $PROTOC --cpp_out=dllexport_decl=LIBPROTOBUF_EXPORT:$CPP_PATH --proto_path=$PROTO_PATH $file
done

