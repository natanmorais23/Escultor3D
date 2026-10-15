
cmake_minimum_required(VERSION 3.16)

project(Escultor3D LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED
    COMPONENTS Core Widgets LinguistTools
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED
    COMPONENTS Core Widgets LinguistTools
)

set(TS_FILES Escultor3D_pt_BR.ts)

add_executable(Escultor3D
    main.cpp
    ${TS_FILES}

    sculptor.h
    sculptor.cpp

    figurageometrica.h
    figurageometrica.cpp

    putvoxel.h
    putvoxel.cpp

    cutvoxel.h
    cutvoxel.cpp

    putbox.h
    putbox.cpp

    cutbox.h
    cutbox.cpp

    putsphere.h
    putsphere.cpp

    cutsphere.h
    cutsphere.cpp

    putellipsoid.h
    putellipsoid.cpp

    cutellipsoid.h
    cutellipsoid.cpp

    interpretador.h
    interpretador.cpp
    mainwindow.h mainwindow.cpp
    sculptorview.h sculptorview.cpp
)

target_link_libraries(Escultor3D
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

if(COMMAND qt_create_translation)
    qt_create_translation(
        QM_FILES
        ${CMAKE_SOURCE_DIR}
        ${TS_FILES}
    )
else()
    qt5_create_translation(
        QM_FILES
        ${CMAKE_SOURCE_DIR}
        ${TS_FILES}
    )
endif()

include(GNUInstallDirs)

install(TARGETS Escultor3D
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
