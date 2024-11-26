cmake_minimum_required(VERSION 3.16)
project(MyQtApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(Qt6Charts_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt6Charts")


# Find the necessary Qt modules
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Charts
)

# Add your application's source files
add_executable(MyQtApp main.cpp MainWindow.cpp User.cpp DataBase.cpp SignWindow.cpp CurrencyConverter.cpp
        Validate.cpp
        Validate.h
        Budget.cpp
        Budget.h)

# Link Qt libraries to your app
target_link_libraries(MyQtApp
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Charts
)