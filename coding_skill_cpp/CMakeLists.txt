cmake_minimum_required(VERSION 3.15)
project(CodingSkillCpp CXX)

# C++17 표준 사용
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 컴파일 옵션
if(MSVC)
    add_compile_options(/W4 /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 모든 .cpp 파일 찾기
file(GLOB PATTERN_SOURCES "*.cpp")

# 각 .cpp 파일을 개별 실행 파일로 컴파일
foreach(SOURCE ${PATTERN_SOURCES})
    get_filename_component(EXEC_NAME ${SOURCE} NAME_WE)
    add_executable(${EXEC_NAME} ${SOURCE})
    
    # 출력 디렉토리 설정
    set_target_properties(${EXEC_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endforeach()

# 정보 출력
message(STATUS "Found ${CMAKE_MATCH_COUNT} pattern files")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
