#!/bin/bash
#Creating deb package from binary

echo "Creating debian package from binary."

BINARY_NAME=
BINARY_VERSION=
INSTALL_TO_DIR=
BINARY_PATH=

TEMPORARY_DIR=.

ARCHITECTURE=

process_option()
{
	case $1 in
	
	--install-path=*)
		INSTALL_TO_DIR="${1#--install-path=}"
	;;

	--binary-name=*)
		BINARY_NAME="${1#--binary-name=}"
	;;

	--binary-version=*)
		BINARY_VERSION="${1#--binary-version=}"
	;;

	--binary-path=*)
		BINARY_PATH="${1#--binary-path=}"
	;;

	*)
		echo "unknown option $1"
		exit 1
	;;
	esac
}

echo "Command line options: $* ..." #keep it for build server logs

for arg in $@; do process_option $arg ; done

echo "Binary name: " ${BINARY_NAME}
echo "Binary version: " ${BINARY_VERSION}
echo "Binary path: " ${BINARY_PATH}
echo "Install path: " ${INSTALL_TO_DIR}



[ -z "${BINARY_NAME}" ] && { echo "Binary name not provided!"; exit 1; }
[ -z "${BINARY_VERSION}" ] && { echo "Binary version not provided!"; exit 1; }
[ -z "${BINARY_PATH}" ] && { echo "Binary path not provided!"; exit 1; }
[ -z "${INSTALL_TO_DIR}" ] && { echo "Installation path not provided!"; exit 1; }

#Checking for dpkg
command -v dpkg-deb >/dev/null 2>&1 || { echo "Deb-preparation script requires dpkg-deb, but it's not installed.  Aborting."; exit 1; }

#Setting up hardware platform
ARCHITECTURE=$(command dpkg-architecture -q DEB_BUILD_ARCH 2>&1 || { echo "Deb-preparation script can't define hardware platform. Aborting."; exit 1; })

[ -z "${ARCHITECTURE}" ] && { echo "Hardware architecture not defined!"; exit 1; }

#Prepare directory

PACKAGE_DIR=${TEMPORARY_DIR}/${BINARY_NAME}_${BINARY_VERSION}_${ARCHITECTURE}
mkdir ${PACKAGE_DIR} || { echo "Can't create temporary directory: ${PACKAGE_DIR}"; }

#Prepare directory where the binary should be installed on the root fs
INSTALL_LOCATION=/usr/local/bin

mkdir -p ${PACKAGE_DIR}/${INSTALL_LOCATION} || { echo "Can't create temporary directory: ${PACKAGE_DIR}/${INSTALL_LOCATION}"; }

#Copy binary to installation dir
#Try copy ./helloworld to ./helloworld_1.1.1_arm64//usr/local/bin
echo "Try copy ${BINARY_PATH}/${BINARY_NAME} to ${PACKAGE_DIR}${INSTALL_LOCATION}"
cp ${BINARY_PATH}/${BINARY_NAME} ${PACKAGE_DIR}${INSTALL_LOCATION} || { echo "Can't copy ${BINARY_PATH}/${BINARY_NAME} to ${PACKAGE_DIR}${INSTALL_LOCATION}"; }

#Prepare directory for DEB-control file
mkdir ${PACKAGE_DIR}/DEBIAN || { echo "Can't create temporary directory: ${PACKAGE_DIR}/DEBIAN"; }

#Create control file
touch ${PACKAGE_DIR}/DEBIAN/control || { echo "Can't create temporary file: ${PACKAGE_DIR}/DEBIAN/control"; }

CONTROL_FILE_TEMPLATE="Package: ${BINARY_NAME}\nVersion: ${BINARY_VERSION}\nArchitecture: ${ARCHITECTURE}\nMaintainer: Mar94oK <maxim.uvarenkov@gs-labs.ru>\nDescription: A program that greets you. OTUS-Course HW-1\n
 And obviously nothing else!"

echo "Debian package control will be: "
echo -e ${CONTROL_FILE_TEMPLATE} | tee ${PACKAGE_DIR}/DEBIAN/control

#Create debian package
DEBIAN_PACKAGE_FOLDER=${BINARY_NAME}_${BINARY_VERSION}_${ARCHITECTURE}
dpkg-deb --build --root-owner-group ${DEBIAN_PACKAGE_FOLDER}


echo "Done!"