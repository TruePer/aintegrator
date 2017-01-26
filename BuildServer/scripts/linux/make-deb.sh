#!/bin/bash

#source functions.sh

# Собирает deb-пакет
make_deb() {
	CurrentPath=$1

	pushd ${CurrentPath}/dpkg

	# Specify name and directories -----------------------------------------------------------------
	HG_VERSION=`hg log -r . --template {latesttag}`
	RevisionVer=`hg id -n | sed -r $'s,\+,,g'`

	FULL_VERSION="${HG_VERSION}.${RevisionVer}"
	
	prepare_deb $CurrentPath $FULL_VERSION $RevisionVer

	PACKAGE_NAME=AIntegrator
	PACKAGE_NAME_FINAL="AIntegrator-$FULL_VERSION"
	REL_DIR=${CurrentPath}/../Bin
	DEB_DIR=${PACKAGE_NAME}
	DEB_PACKAGE_DIR=${DEB_DIR}/opt/${PACKAGE_NAME}

	# Prepare files --------------------------------------------------------------------------------
	rm -fr ${DEB_DIR}/opt

	mkdir -p ${DEB_PACKAGE_DIR}
	cp -r ${REL_DIR}/* ${DEB_PACKAGE_DIR}
	
	fakeroot dpkg-deb --build ${DEB_DIR}
	dpkg-name *.deb

	DEST_DIR=${REL_DIR}/../packages
	mkdir -p ${DEST_DIR}
	mv *.deb ${DEST_DIR}/

	popd
}

prepare_deb() {
	CurrentPath=$1
	FullVersion=$2
	
	ControlFile=$CurrentPath/dpkg/AIntegrator/DEBIAN/control
	
	echo "Package: aintegrator" > $ControlFile
	echo "Version: $HG_VERSION-$RevisionVer" >> $ControlFile
	echo "Maintainer: http://www.nppntt.ru" >> $ControlFile
	echo "Architecture: amd64" >> $ControlFile
	echo "Section: misc" >> $ControlFile
	echo "Description: AIntegrator" >> $ControlFile
	echo "Depends: qt-5.6.0-m (>= 5.6.0-2)" >> $ControlFile
}

