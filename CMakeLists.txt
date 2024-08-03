cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("iat-editor")
add_executable (iat-editor "src/main.cpp"   "src/process_ut.h" "src/process_ut.cpp" "src/dll_ut.h" "src/dll_loader.cpp" "src/err.h" "src/err.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET iat-editor PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
