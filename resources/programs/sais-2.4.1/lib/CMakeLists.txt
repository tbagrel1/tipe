include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include"
                    "${CMAKE_CURRENT_BINARY_DIR}/../include")

## libsais ##
add_library(sais sais.c)
install(TARGETS sais
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
set_target_properties(sais PROPERTIES
                      VERSION "${LIBRARY_VERSION_FULL}"
                      SOVERSION "${LIBRARY_VERSION_MAJOR}"
                      DEFINE_SYMBOL SAIS_BUILD_DLL
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../examples")

## libsais64 ##
if (BUILD_SAIS64)
    add_library(sais64 sais64.c)
    install(TARGETS sais64
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
    set_target_properties(sais64 PROPERTIES
                          VERSION "${LIBRARY_VERSION_FULL}"
                          SOVERSION "${LIBRARY_VERSION_MAJOR}"
                          DEFINE_SYMBOL SAIS_BUILD_DLL
                          COMPILE_FLAGS "-DSAIS_BUILD_64BIT"
                          RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../examples")
endif (BUILD_SAIS64)
