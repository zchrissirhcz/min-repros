add_library(hello SHARED IMPORTED GLOBAL)

set(hello_proj_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../hello")
set_target_properties(hello PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${hello_proj_dir}"
  IMPORTED_LOCATION "${hello_proj_dir}/build/vs2022-x64/Debug/hello.dll"
  IMPORTED_IMPLIB "${hello_proj_dir}/build/vs2022-x64/Debug/hello.lib"
)