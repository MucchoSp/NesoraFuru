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

# Allow the bundled hts_engine_API CMake file to use its original minimum version.
set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

add_subdirectory(lib/json)
add_subdirectory(lib/hts_engine_API/src)
add_subdirectory(lib/open_jtalk/src)
target_include_directories(openjtalk PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/jpcommon
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/mecab/src
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/mecab2njd
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/njd
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/njd2jpcommon
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/njd_set_accent_phrase
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/njd_set_accent_type
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/njd_set_digit
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/njd_set_long_vowel
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/njd_set_pronunciation
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/njd_set_unvoiced_vowel
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/open_jtalk/src/text2mecab
)

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (NesoraFuru "src/main.cpp" "src/NesoraProsody.cpp" "src/NesoraGenericClass.cpp" "src/NesoraSynthesize.cpp" "src/NesoraVoice.cpp" "src/NesoraFuru.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NesoraFuru PROPERTY CXX_STANDARD 20)
endif()

if (MSVC)
  target_compile_options(NesoraFuru PRIVATE /utf-8)
endif()

target_include_directories(NesoraFuru PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/hts_engine_API/src/include
)

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。

target_link_libraries(NesoraFuru PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(NesoraFuru PRIVATE openjtalk)
target_link_libraries(NesoraFuru PRIVATE hts_engine_API)
