cmake_minimum_required(VERSION 3.5)
project(Boids_Trenton_colton)

FIND_PACKAGE(Qt5 COMPONENTS GUI Widgets 3DRender 3DInput 3DExtras 3DCore Concurrent REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)


add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindowform.ui
    boidphysics.cpp
    boidphysics.h
    myvector.cpp
    myvector.h
    trackballcameracontroller.cpp
    trackballcameracontroller.h
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Gui
    Qt5::3DRender
    Qt5::3DCore
    Qt5::3DInput
    Qt5::3DExtras
    Qt5::Concurrent
)

#add_library(
#    myvector.cpp
#    myvector.h
#    boidphysics.cpp
#    boidphysics.h
#    )

FIND_PACKAGE(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(Boids_Trenton_colton_vector_unitTest
    MyVectorUnitTest.cpp
    )


target_link_libraries(Boids_Trenton_colton_vector_unitTest  ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Qt5::3DCore)


add_executable(Boids_Trenton_colton_physics_unitTest
    BoidPhysicsUnitTest.cpp
    )

target_link_libraries(Boids_Trenton_colton_physics_unitTest  ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Qt5::3DCore)
