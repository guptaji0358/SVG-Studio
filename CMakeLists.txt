cmake_minimum_required(VERSION 3.21)

project(SVGStudio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets SvgWidgets)

qt_standard_project_setup()

qt_add_executable(SVGStudio WIN32
    SVG_Studio.cpp
    SVG_STUDIO_APP.rc
)

qt_add_resources(SVGStudio app_resources
    PREFIX "/"
    FILES
        NEW_FILE_ICON.svg
        OPEN_FILE_ICON.svg
        OPEN_FOLDER_ICON.svg
        CONVERT_TO_SVG.svg
        EDIT_BUTTON_ICON.svg
        DOWN_ARROW_ICON.svg
        RIGHT_ARROW_ICON.svg
        1_SVG_DRAG_DROP_ANIMATION.gif
        2_SVG_DRAG_DROP_ANIMATION.gif
        3_AND_MORE_SVG_DRAG_DROP_ANIMATION.gif
        NORMAL_REMOVE.svg
        RED_REMOVE.svg
        GLOW_RED_REMOVE.svg
)

target_link_libraries(SVGStudio PRIVATE
    Qt6::Widgets
    Qt6::SvgWidgets
)

set_target_properties(SVGStudio PROPERTIES
    OUTPUT_NAME "SVG Studio"
)

set_target_properties(SVGStudio PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)