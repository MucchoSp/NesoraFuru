# CMakeList.txt : NesoraFuru の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.10)

# サポートされている場合は、MSVC コンパイラのホット リロードを有効にします。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("NesoraFuru")

add_subdirectory(lib/json)

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (NesoraFuru "src/NesoraFuru.cpp" "src/NesoraFuru.h" "src/main.cpp" "src/NesoraWave.h" "src/FujisakiModel.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NesoraFuru PROPERTY CXX_STANDARD 20)
endif()

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。

target_link_libraries(NesoraFuru PRIVATE nlohmann_json::nlohmann_json)
