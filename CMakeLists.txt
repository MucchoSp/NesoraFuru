cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(NesoraFuru LANGUAGES C CXX)

# 古い hts_engine_API の CMake 設定を CMake 4 でも読み込む。
set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

add_subdirectory(lib/json)

# Open JTalk と hts_engine API を共有ライブラリに組み込む。
function(nesora_furu_add_native_dependencies)
  set(BUILD_SHARED_LIBS OFF)
  add_subdirectory(lib/hts_engine_API/src)
  add_subdirectory(lib/open_jtalk/src)
endfunction()

nesora_furu_add_native_dependencies()
set_target_properties(hts_engine_API openjtalk
  PROPERTIES POSITION_INDEPENDENT_CODE ON
)

add_library(NesoraFuruLib SHARED
  src/NesoraFuru.cpp
  src/NesoraGenericClass.cpp
  src/NesoraProsody.cpp
  src/NesoraSynthesize.cpp
  src/NesoraVoice.cpp
  src/NesoraCAPI.cpp)
add_library(NesoraFuru::NesoraFuru ALIAS NesoraFuruLib)

set_target_properties(NesoraFuruLib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_compile_features(NesoraFuruLib PUBLIC cxx_std_20)

set(open_jtalk_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src")
target_include_directories(NesoraFuruLib
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/hts_engine_API/src/include"
    "${open_jtalk_source_dir}/jpcommon"
    "${open_jtalk_source_dir}/mecab/src"
    "${open_jtalk_source_dir}/mecab2njd"
    "${open_jtalk_source_dir}/njd"
    "${open_jtalk_source_dir}/njd2jpcommon"
    "${open_jtalk_source_dir}/njd_set_accent_phrase"
    "${open_jtalk_source_dir}/njd_set_accent_type"
    "${open_jtalk_source_dir}/njd_set_digit"
    "${open_jtalk_source_dir}/njd_set_long_vowel"
    "${open_jtalk_source_dir}/njd_set_pronunciation"
    "${open_jtalk_source_dir}/njd_set_unvoiced_vowel"
    "${open_jtalk_source_dir}/text2mecab"
)

target_link_libraries(NesoraFuruLib
  PUBLIC nlohmann_json::nlohmann_json
  PRIVATE openjtalk hts_engine_API
)

add_executable(NesoraFuru src/main.cpp)
target_link_libraries(NesoraFuru PRIVATE NesoraFuru::NesoraFuru)

if(MSVC)
  target_compile_options(NesoraFuruLib PRIVATE /utf-8)
  target_compile_options(NesoraFuru PRIVATE /utf-8)
endif()