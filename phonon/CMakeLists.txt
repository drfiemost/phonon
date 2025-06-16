if(NOT PHONON_NO_GRAPHICSVIEW)
    message(STATUS "Building graphicsview.")
    add_subdirectory(graphicsview)
else()
    message(STATUS "Not Building graphicsview.")
    add_definitions(-DPHONON_NO_GRAPHICSVIEW)
endif()

if (PHONON_BUILD_EXPERIMENTAL)
    add_subdirectory(experimental)
endif (PHONON_BUILD_EXPERIMENTAL)

# ------------------------ Configure File QMake Style ------------------------ #

if (PHONON_NO_DBUS OR NOT QT_QTDBUS_FOUND)
    set(PHONON_NO_DBUS_DEFINE "#define PHONON_NO_DBUS")
else()
    set(PHONON_NO_DBUS_DEFINE "/* #undef PHONON_NO_DBUS */")
endif()

if (PHONON_NO_CAPTURE)
    set(PHONON_NO_CAPTURE_DEFINE "#define PHONON_NO_CAPTURE")
else()
    set(PHONON_NO_CAPTURE_DEFINE "/* #undef PHONON_NO_CAPTURE */")
endif()

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/phononconfig_p.h.in PHONONCONFIG_FILE)
string(REGEX REPLACE "\\#include( )+\\\\\"([A-Za-z_]+\\.h)\\\\\"" "#include \"\\2\"" PHONONCONFIG_FILE "${PHONONCONFIG_FILE}")
string(REGEX REPLACE "\\#define( )+([A-Za-z_]+)( )+\\\\\"(.+)\\\\\"" "#define \\2 \"\\4\"" PHONONCONFIG_FILE "${PHONONCONFIG_FILE}")
string(REGEX REPLACE "\\$\\$\\{(PHONON_NO_DBUS_DEFINE)\\}" "${PHONON_NO_DBUS_DEFINE}" PHONONCONFIG_FILE "${PHONONCONFIG_FILE}")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/phononconfig_p.h "${PHONONCONFIG_FILE}")
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/phononconfig_p.h)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/phononnamespace.h.in PHONONNAMESPACE_FILE)
string(REGEX REPLACE "\\#include( )+\\\\\"([A-Za-z_]+\\.h)\\\\\"" "#include \"\\2\"" PHONONNAMESPACE_FILE "${PHONONNAMESPACE_FILE}")
string(REGEX REPLACE "\\#define( )+([A-Za-z_]+)( )+\\\\\"(.+)\\\\\"" "#define \\2 \"\\4\"" PHONONNAMESPACE_FILE "${PHONONNAMESPACE_FILE}")
string(REGEX REPLACE "\\$\\$\\{PHONON_LIB_MAJOR_VERSION\\}" "${PHONON_LIB_MAJOR_VERSION}" PHONONNAMESPACE_FILE "${PHONONNAMESPACE_FILE}")
string(REGEX REPLACE "\\$\\$\\{PHONON_LIB_MINOR_VERSION\\}" "${PHONON_LIB_MINOR_VERSION}" PHONONNAMESPACE_FILE "${PHONONNAMESPACE_FILE}")
string(REGEX REPLACE "\\$\\$\\{PHONON_LIB_PATCH_VERSION\\}" "${PHONON_LIB_PATCH_VERSION}" PHONONNAMESPACE_FILE "${PHONONNAMESPACE_FILE}")
string(REGEX REPLACE "\\$\\$\\{PHONON_NO_CAPTURE_DEFINE\\}" "${PHONON_NO_CAPTURE_DEFINE}" PHONONNAMESPACE_FILE "${PHONONNAMESPACE_FILE}")
# We write these to a strange place so the demos can find them properly
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/phononnamespace.h "${PHONONNAMESPACE_FILE}")
# Required for demos/cmake/FindPhonon.cmake
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/../includes/phonon/phononnamespace.h "${PHONONNAMESPACE_FILE}")
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/phononnamespace.h)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/../includes/phonon/phononnamespace.h )

# ---------------------------------------------------------------------------- #

include_directories(${CMAKE_BINARY_DIR}/includes/phonon)

set(phonon_LIB_SRCS
   abstractaudiooutput.cpp
   abstractaudiooutput_p.cpp
   abstractmediastream.cpp
   abstractvideooutput.cpp
   abstractvideooutput_p.cpp
   audiodataoutput.cpp
   audiooutput.cpp
   audiooutputinterface.cpp
   backendcapabilities.cpp
   effect.cpp
   effectparameter.cpp
   effectwidget.cpp
   factory.cpp
   globalconfig.cpp
   iodevicestream.cpp
   mediacontroller.cpp
   mediaobject.cpp
   medianode.cpp
   mediasource.cpp
   mrl.cpp
   objectdescription.cpp
   objectdescriptionmodel.cpp
   path.cpp
   phononnamespace.cpp
   platform.cpp
   seekslider.cpp
   statesvalidator.cpp
   streaminterface.cpp
   swiftslider.cpp
   volumefadereffect.cpp
   volumeslider.cpp
   videoplayer.cpp
   videowidget.cpp
   ${phonon_VGO_SRCS}
   )

if(ECM_FOUND)
    ecm_create_qm_loader(phonon_LIB_SRCS libphonon_qt)
endif()

if (QT_QTDBUS_FOUND AND NOT PHONON_NO_DBUS)
   list(APPEND phonon_LIB_SRCS
      audiooutputadaptor.cpp
   )
endif (QT_QTDBUS_FOUND AND NOT PHONON_NO_DBUS)

option(PHONON_NO_PLATFORMPLUGIN "Do not use any platform plugin")
if (PHONON_NO_PLATFORMPLUGIN)
    add_definitions(-DQT_NO_PHONON_PLATFORMPLUGIN)
endif (PHONON_NO_PLATFORMPLUGIN)

add_definitions(-DPHONON_LIBRARY_PATH="${CMAKE_INSTALL_PREFIX}/${PLUGIN_INSTALL_DIR}/plugins")
add_definitions(-DPHONON_BACKEND_DIR_SUFFIX="/${PHONON_LIB_SONAME}_backend/")

add_library(${PHONON_LIB_SONAME} SHARED ${phonon_LIB_SRCS})
qt5_use_modules(${PHONON_LIB_SONAME} Core Widgets)

if(QT_QTDBUS_FOUND AND NOT PHONON_NO_DBUS)
    qt5_use_modules(${PHONON_LIB_SONAME} DBus)
endif(QT_QTDBUS_FOUND AND NOT PHONON_NO_DBUS)

if(NOT PHONON_NO_GRAPHICSVIEW)
    qt5_use_modules(${PHONON_LIB_SONAME} OpenGL)
    target_link_libraries(${PHONON_LIB_SONAME} ${OPENGL_gl_LIBRARY})
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
  # We need to explicitly link libm to phonon in Solaris
  target_link_libraries(${PHONON_LIB_SONAME} m)
endif (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")

set_target_properties(${PHONON_LIB_SONAME} PROPERTIES
                                           VERSION ${PHONON_LIB_VERSION}
                                           SOVERSION ${PHONON_LIB_SOVERSION}
                                           DEFINE_SYMBOL MAKE_PHONON_LIB)

target_include_directories(${PHONON_LIB_SONAME}
    INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>")

install(TARGETS ${PHONON_LIB_SONAME} EXPORT PhononLibs ${INSTALL_TARGETS_DEFAULT_ARGS} )

install(FILES
   abstractaudiooutput.h
   abstractmediastream.h
   abstractvideooutput.h
   addoninterface.h
   audiodataoutput.h
   audiodataoutputinterface.h
   audiooutput.h
   audiooutputinterface.h
   backendcapabilities.h
   backendinterface.h
   effect.h
   effectinterface.h
   effectparameter.h
   effectwidget.h
   globalconfig.h
   globaldescriptioncontainer.h
   mediacontroller.h
   medianode.h
   mediaobject.h
   mediaobjectinterface.h
   mediasource.h
   mrl.h
   objectdescription.h
   objectdescriptionmodel.h
   path.h
   phonon_export.h
   phonondefs.h
   platformplugin.h
   seekslider.h
   streaminterface.h
   videoplayer.h
   videowidget.h
   videowidgetinterface.h
   volumefadereffect.h
   volumefaderinterface.h
   volumeslider.h
   ${phonon_VGO_HDRS}
   ${CMAKE_CURRENT_BINARY_DIR}/phononnamespace.h
   DESTINATION ${INCLUDE_INSTALL_DIR}/phonon COMPONENT Devel)

if (QT_QTDBUS_FOUND AND NOT PHONON_NO_DBUS)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/org.kde.Phonon.AudioOutput.xml ${CMAKE_CURRENT_BINARY_DIR}/org.kde.${PHONON_LIB_SONAME_CAMEL}.AudioOutput.xml @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.${PHONON_LIB_SONAME_CAMEL}.AudioOutput.xml DESTINATION  ${DBUS_INTERFACES_INSTALL_DIR})
endif (QT_QTDBUS_FOUND AND NOT PHONON_NO_DBUS)

