add_library(hello SHARED IMPORTED GLOBAL)

set(hello_proj_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../hello")
set_target_properties(hello PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${hello_proj_dir}"
  IMPORTED_LOCATION_DEBUG "${hello_proj_dir}/build/vs2022-x64/Debug/hello.dll"
  IMPORTED_LOCATION_RELEASE "${hello_proj_dir}/build/vs2022-x64/Release/hello.dll"
  IMPORTED_LOCATION_MINSIZEREL "${hello_proj_dir}/build/vs2022-x64/Release/hello.dll"
  IMPORTED_LOCATION_RELWITHDEBINFO "${hello_proj_dir}/build/vs2022-x64/Release/hello.dll"
  IMPORTED_IMPLIB_DEBUG "${hello_proj_dir}/build/vs2022-x64/Debug/hello.lib"
  IMPORTED_IMPLIB_RELEASE "${hello_proj_dir}/build/vs2022-x64/Release/hello.lib"
  IMPORTED_IMPLIB_MINSIZEREL "${hello_proj_dir}/build/vs2022-x64/Release/hello.lib"
  IMPORTED_IMPLIB_RELWITHDEBINFO "${hello_proj_dir}/build/vs2022-x64/Release/hello.lib"
)