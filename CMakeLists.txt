cmake_minimum_required(VERSION 3.16)
project(MyQtApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the necessary Qt modules
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
)

# Add your application's source files
add_executable(MyQtApp main.cpp MainWindow.cpp User.cpp DataBase.cpp SignWindow.cpp CurrencyConverter.cpp)

# Link Qt libraries to your app
target_link_libraries(MyQtApp
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
