cmake_minimum_required(VERSION 3.21)

project(SVGStudio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    Qt6 REQUIRED
    COMPONENTS
        Widgets
        SvgWidgets
        Network
)

find_package(OpenCV REQUIRED)

qt_standard_project_setup()

qt_add_executable(SVGStudio WIN32
    SVG_Studio.cpp
    SVG_STUDIO_APP.rc
)

qt_add_resources(SVGStudio app_resources
    PREFIX "/"
    FILES
        Assets/NEW_FILE_ICON.svg
        Assets/OPEN_FILE_ICON.svg
        Assets/OPEN_FOLDER_ICON.svg
        Assets/CONVERT_TO_SVG.svg
        Assets/EDIT_BUTTON_ICON.svg
        Assets/DOWN_ARROW_ICON.svg
        Assets/RIGHT_ARROW_ICON.svg
        Assets/1_SVG_DRAG_DROP_ANIMATION.gif
        Assets/2_SVG_DRAG_DROP_ANIMATION.gif
        Assets/3_AND_MORE_SVG_DRAG_DROP_ANIMATION.gif
        Assets/NORMAL_REMOVE.svg
        Assets/RED_REMOVE.svg
        Assets/GLOW_RED_REMOVE.svg
        Assets/DARK_MODE_SVG_FILE_ICON.svg
        Assets/LIGHT_MODE_SVG_FILE_ICON.svg
        Assets/LIGHT_MODE_SVG_FILE_ICON.svg
        DARK_MODE_SVG_FILE_ICON.ico
        LIGHT_MODE_SVG_FILE_ICON.ico
)

target_link_libraries(SVGStudio PRIVATE
    Qt6::Widgets
    Qt6::SvgWidgets
    Qt6::Network
    ${OpenCV_LIBS}
)

set_target_properties(SVGStudio PROPERTIES
    OUTPUT_NAME "SVG Studio"
)

set_target_properties(SVGStudio PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)