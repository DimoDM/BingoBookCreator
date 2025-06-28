# BingoBookCreator

cmake_minimum_required(VERSION 3.15)
project(BingoBookCreator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Define absolute paths ===
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/BingoBookCreator)
set(PDF_SUPPORT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../PDF_Support/Source)
set(PDF_SUPPORT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../PDF_Support/Include)

# === Collect project and library source files ===
file(GLOB_RECURSE APP_SOURCES
    ${SOURCE_DIR}/*.cpp
)

file(GLOB_RECURSE PDF_SOURCES
    ${PDF_SUPPORT_SOURCE_DIR}/*.c
    ${PDF_SUPPORT_SOURCE_DIR}/*.cpp
)

# === Build PDF support as static library ===
add_library(pdf_support STATIC ${PDF_SOURCES})

# === Ensure header files are available to .c/.cpp files ===
target_include_directories(pdf_support
    PUBLIC ${PDF_SUPPORT_INCLUDE_DIR}
)

# === Build the app ===
add_executable(BingoBookCreator ${APP_SOURCES})

# === Make sure app can also see headers from the PDF library ===
target_include_directories(BingoBookCreator
    PRIVATE
        ${SOURCE_DIR}
        ${PDF_SUPPORT_INCLUDE_DIR}
)

# === Link PDF library with app ===
target_link_libraries(BingoBookCreator PRIVATE pdf_support)
