#!/bin/bash

echo "------------ Purge 3gpp-gui project ------------------"

echo "3gpp-gui"
BASEDIR=`dirname $0`
CURRENT_PATH=`cd $BASEDIR; pwd`

pushd $CURRENT_PATH/../..
hg purge --all
popd

echo "QmlMaterial"
pushd $CURRENT_PATH/../../External/QmlMaterial
hg purge --all
popd

echo "Protocol"
pushd $CURRENT_PATH/../../Sources/AIntegrator/Ai_Kernel/Protocol/proto
hg purge --all
popd
