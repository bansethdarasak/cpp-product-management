cmake_minimum_required(VERSION 3.14)

project(ProductManagementApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# xlnt
FetchContent_Declare(
    xlnt
    GIT_REPOSITORY https://github.com/tfussell/xlnt.git
    GIT_TAG origin/master
)

FetchContent_MakeAvailable(xlnt)

# tabulate
FetchContent_Declare(
    tabulate
    GIT_REPOSITORY https://github.com/p-ranav/tabulate.git
    GIT_TAG master
)

FetchContent_MakeAvailable(tabulate)

# Executable
add_executable(ProductManagementApp
    main.cpp
    src/Product.cpp
    src/User.cpp
    src/FileUtils.cpp
    src/MenuUtils.cpp
    src/InputUtils.cpp
    src/AuthManager.cpp
    src/ProductCatalog.cpp
)

# Include headers
target_include_directories(ProductManagementApp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Link libraries
target_link_libraries(ProductManagementApp PRIVATE xlnt tabulate)