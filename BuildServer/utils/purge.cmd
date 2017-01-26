@echo off

echo ------------ Purge 3gpp-gui project ------------------

set CURRENT_PATH=%~dp0

echo 3gpp-gui 
pushd %CURRENT_PATH%\..\..
hg purge --all
popd

echo QmlMaterial
pushd %CURRENT_PATH%\..\..\External\QmlMaterial
hg purge --all
popd

echo Protocol
pushd %CURRENT_PATH%\..\..\Sources\AIntegrator\Ai_Kernel\Protocol\proto
hg purge --all
popd
