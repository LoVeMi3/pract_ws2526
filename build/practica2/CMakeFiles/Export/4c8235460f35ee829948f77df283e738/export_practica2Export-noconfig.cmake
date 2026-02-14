#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "practica2::practica2" for configuration ""
set_property(TARGET practica2::practica2 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(practica2::practica2 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpractica2.a"
  )

list(APPEND _cmake_import_check_targets practica2::practica2 )
list(APPEND _cmake_import_check_files_for_practica2::practica2 "${_IMPORT_PREFIX}/lib/libpractica2.a" )

# Import target "practica2::teleopt" for configuration ""
set_property(TARGET practica2::teleopt APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(practica2::teleopt PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/practica2/teleopt"
  )

list(APPEND _cmake_import_check_targets practica2::teleopt )
list(APPEND _cmake_import_check_files_for_practica2::teleopt "${_IMPORT_PREFIX}/lib/practica2/teleopt" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
