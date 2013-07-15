#
# Configuration file for CMakeLists.txt files
#
# Author : Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
#
# This file is included by the main CMakeLists.txt file, and defines variables
# to configure the build and install 
#
# The variables in this file can also be overriden through the  
# CMake.in.local.config file 
#


################### general ###################

SET(OPENFLUID_MAIN_NAME "openfluid")

SET(OPENFLUID_CMD_APP "openfluid")

SET(OPENFLUID_RELATIVEDIR "${OPENFLUID_MAIN_NAME}")

SET(OPENFLUID_DEFAULT_CONFIGFILE "openfluid.conf")

SET(OPENFLUID_INPUTDIR "OPENFLUID.IN")
SET(OPENFLUID_OUTPUTDIR "OPENFLUID.OUT")
SET(OPENFLUID_SIMPLUGSDIR "simulators")
SET(OPENFLUID_OBSPLUGSDIR "observers")
SET(OPENFLUID_EXAMPLESDIR "examples")
SET(OPENFLUID_PROJECTSDIR "projects")

SET(OPENFLUID_SIMSMAXNUMTHREADS "4")

SET(OPENFLUID_PROJECT_FILE ".openfluidprj")
SET(OPENFLUID_PROJECT_INPUTDIR "IN")
SET(OPENFLUID_PROJECT_OUTPUTDIRPREFIX "OUT")


################### install path ###################

SET(BIN_INSTALL_PATH "bin")
SET(LIB_INSTALL_PATH "lib${OF_LIBDIR_SUFFIX}")
SET(INCLUDE_INSTALL_PATH "include")
SET(SHARE_INSTALL_PATH "share")
SET(INCLUDE_OPENFLUID_INSTALL_PATH "${INCLUDE_INSTALL_PATH}/openfluid")
SET(SHARE_OPENFLUID_INSTALL_PATH "${SHARE_INSTALL_PATH}/openfluid")
SET(SHARE_DESKTOPENTRY_INSTALL_PATH "${SHARE_INSTALL_PATH}/applications")

SET(SHARE_COMMON_INSTALL_PATH "${SHARE_OPENFLUID_INSTALL_PATH}/common")
SET(SHARE_APPS_INSTALL_PATH "${SHARE_OPENFLUID_INSTALL_PATH}/apps")
SET(SIMULATORS_INSTALL_PATH "${LIB_INSTALL_PATH}/openfluid/simulators")
SET(OBSERVERS_INSTALL_PATH "${LIB_INSTALL_PATH}/openfluid/observers")
SET(BUILDEREXT_INSTALL_PATH "${LIB_INSTALL_PATH}/openfluid/builderext")

SET(PKGCONFIG_INSTALL_PATH "${LIB_INSTALL_PATH}/pkgconfig")

IF (WIN32)
  SET(CMAKE_MODULES_INSTALL_PATH "${LIB_INSTALL_PATH}/cmake")
ELSE()
  SET(CMAKE_MODULES_INSTALL_PATH "${LIB_INSTALL_PATH}/openfluid/cmake")
ENDIF()  

SET(DOC_INSTALL_PATH "${SHARE_INSTALL_PATH}/doc/openfluid")
SET(MAIN_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/main")
SET(SIMULATORS_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/simulators")
SET(OBSERVERS_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/observers")
SET(EXAMPLES_INSTALL_PATH "${DOC_INSTALL_PATH}/examples")
SET(EXAMPLES_SRC_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}/src")
SET(EXAMPLES_DATA_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}/datasets")
SET(EXAMPLES_PROJECTS_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}/projects")
SET(KERNEL_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}")
SET(MANUALS_DOC_INSTALL_PATH "${DOC_INSTALL_PATH}/manuals")
SET(MANUALS_DOCPDF_INSTALL_PATH "${MANUALS_DOC_INSTALL_PATH}/pdf")
SET(MANUALS_DOCHTML_INSTALL_PATH "${MANUALS_DOC_INSTALL_PATH}/html")
SET(EXAMPLES_DOCPDF_INSTALL_PATH "${EXAMPLES_INSTALL_PATH}")

################### source and output path ###################

SET(BUILD_OUTPUT_PATH "${CMAKE_BINARY_DIR}/dist")

SET(LIB_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${LIB_INSTALL_PATH}")
SET(SIMULATORS_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${SIMULATORS_INSTALL_PATH}")
SET(OBSERVERS_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${OBSERVERS_INSTALL_PATH}")
SET(BUILDEREXT_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${BUILDEREXT_INSTALL_PATH}")
SET(BIN_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${BIN_INSTALL_PATH}")
SET(PKGCONFIG_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${PKGCONFIG_INSTALL_PATH}")
SET(CMAKE_MODULES_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${CMAKE_MODULES_INSTALL_PATH}")

SET(DOC_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${DOC_INSTALL_PATH}")
SET(MAIN_DOC_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${MAIN_DOC_INSTALL_PATH}")
SET(DOC_LAYOUT_DIR "${CMAKE_SOURCE_DIR}/doc/layout")
SET(DOC_CONTENTS_DIR "${CMAKE_SOURCE_DIR}/doc/contents")
SET(DOC_SOURCES_DIR "${CMAKE_SOURCE_DIR}/src/openfluid")


SET(DOCFORDEV_OUTPUT_PATH "${CMAKE_BINARY_DIR}/docfordev")
SET(DOCFORDEV_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

SET(SIMULATORS_DOC_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${SIMULATORS_DOC_INSTALL_PATH}")

#SET(DOC_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources/doc")
SET(DOC_BUILD_DIR "${CMAKE_BINARY_DIR}/docdir-for-build")
#SET(MANUALS_DOC_BUILD_DIR "${DOC_BUILD_DIR}/manuals")
SET(MANUALS_DOCPDF_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${MANUALS_DOCPDF_INSTALL_PATH}")
SET(MANUALS_DOCHTML_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${MANUALS_DOCHTML_INSTALL_PATH}")

#SET(EXAMPLES_DOC_BUILD_DIR "${DOC_BUILD_DIR}/examples")
#SET(EXAMPLES_DOCPDF_OUTPUT_PATH "${BUILD_OUTPUT_PATH}/${EXAMPLES_DOCPDF_INSTALL_PATH}")
SET(EXAMPLES_PATH "${CMAKE_SOURCE_DIR}/${OPENFLUID_EXAMPLESDIR}")
SET(EXAMPLES_PROJECTS_PATH "${EXAMPLES_PATH}/projects")


SET(OPENFLUID_NLS_LOCALEPATH "${SHARE_INSTALL_PATH}/locale")
SET(PO_BUILD_DIR "${BUILD_OUTPUT_PATH}/${SHARE_INSTALL_PATH}/locale")

################### test paths ###################

SET(TEST_OUTPUT_PATH "${CMAKE_BINARY_DIR}/tests-bin")
SET(TESTS_DATASETS_PATH "${CMAKE_SOURCE_DIR}/resources/tests/datasets")
SET(TESTS_OUTPUTDATA_PATH "${CMAKE_BINARY_DIR}/tests-output")


################### versions ###################

SET(CUSTOM_CMAKE_VERSION "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

SET(VERSION_MAJOR 2)
SET(VERSION_MINOR 0)
SET(VERSION_PATCH 0)
SET(VERSION_STATUS "beta4") # example: SET(VERSION_STATUS "rc1")

SET(FULL_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

IF(VERSION_STATUS)
  SET(FULL_VERSION "${FULL_VERSION}~${VERSION_STATUS}")
ENDIF(VERSION_STATUS)


################### compilation and build ###################
IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC) 
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
  SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -Wextra")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")  
ENDIF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

SET(PLUGINS_BINARY_EXTENSION "${CMAKE_SHARED_LIBRARY_SUFFIX}")

SET(OPENFLUID_SIMULATORS_SUFFIX "_ofware-sim")
SET(OPENFLUID_OBSERVERS_SUFFIX "_ofware-obs")


SET(DEBUG_PREFIX "[OpenFLUID debug]")


################### library build ###################

SET(OPENFLUID_ENABLE_LANDR 1)


################### applications build ###################

SET(OPENFLUID_ENABLE_GUI 1)

# set this to 1 to build openfluid command line program
SET(BUILD_APP_CMD 1)

# set this to 1 to build openfluid-minimal
SET(BUILD_APP_MINIMAL 1)

# set this to 1 to build openfluid-builder
SET(BUILD_APP_BUILDER 1)

# set this to 1 to build openfluid-market-client
SET(BUILD_APP_MARKETCLIENT 0)

# set this to 1 to build openfluid-minimal-gui
SET(BUILD_APP_MINIMALGUI 0)


################### simulators build ###################

# uncomment this to build simulators mixing C++ and fortran source codes (in this source tree)
#SET(BUILD_FORTRAN_SIMULATORS 1)


################### doc build ###################

# uncomment this to build latex docs
SET(BUILD_DOCS 1)


################### logfiles ###################

SET(OPENFLUID_MESSAGES_LOG_FILE "openfluid-messages.log")

SET(OPENFLUID_CUMULATIVE_PROFILE_FILE "openfluid-profile-cumulative.log")
SET(OPENFLUID_SCHEDULE_PROFILE_FILE "openfluid-profile-schedule.log")
SET(OPENFLUID_TIMEINDEX_PROFILE_FILE "openfluid-profile-timeindex.log")


################### market ###################

SET(OPENFLUID_MARKETBAGDIR "market-bag")
SET(OPENFLUID_MARKETPLACE_SITEFILE "OpenFLUID-Marketplace")
SET(OPENFLUID_MARKETPLACE_CATALOGFILE "Catalog")

SET(OPENFLUID_MARKET_COMMONBUILDOPTS "-DSIM_SIM2DOC_DISABLED=1")

IF(WIN32)
  SET(OPENFLUID_MARKET_COMMONBUILDOPTS "-G \\\"${CMAKE_GENERATOR}\\\" ${OPENFLUID_MARKET_COMMONBUILDOPTS}")
ENDIF(WIN32)


################### i18n ###################

# Native Language Support
SET(OPENFLUID_NLS_ENABLE 1)

SET(OPENFLUID_NLS_PACKAGE openfluid)
SET(PO_DIR po)
SET(PO_PACKAGENAME ${OPENFLUID_NLS_PACKAGE})
SET(PO_COPYRIGHTHOLDER "INRA-Montpellier SupAgro")
SET(PO_BUGSADDRESS "libres at supagro.inra.fr")


################### builder ###################


SET(BUILDEREXTENSION_BINARY_EXTENSION "${CMAKE_SHARED_LIBRARY_SUFFIX}bepi")
SET(BUILDER_EXTSDIR "builder-extensions")



