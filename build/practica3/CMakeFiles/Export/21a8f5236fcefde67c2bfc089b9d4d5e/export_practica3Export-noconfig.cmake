#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "practica3::practica3" for configuration ""
set_property(TARGET practica3::practica3 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(practica3::practica3 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpractica3.a"
  )

list(APPEND _cmake_import_check_targets practica3::practica3 )
list(APPEND _cmake_import_check_files_for_practica3::practica3 "${_IMPORT_PREFIX}/lib/libpractica3.a" )

# Import target "practica3::detectObs" for configuration ""
set_property(TARGET practica3::detectObs APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(practica3::detectObs PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/practica3/detectObs"
  )

list(APPEND _cmake_import_check_targets practica3::detectObs )
list(APPEND _cmake_import_check_files_for_practica3::detectObs "${_IMPORT_PREFIX}/lib/practica3/detectObs" )

# Import target "practica3::detectNode3d" for configuration ""
set_property(TARGET practica3::detectNode3d APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(practica3::detectNode3d PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/practica3/detectNode3d"
  )

list(APPEND _cmake_import_check_targets practica3::detectNode3d )
list(APPEND _cmake_import_check_files_for_practica3::detectNode3d "${_IMPORT_PREFIX}/lib/practica3/detectNode3d" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
