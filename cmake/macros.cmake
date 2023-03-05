# ---------------------------------
# - module managment              -
# ---------------------------------

function(add_module
    name
	sources
	includes
	lib)
    
    add_library(${name} ${sources})
    target_include_directories(${name} PUBLIC ${includes})
    target_link_libraries(${name} ${lib})
    target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${name})
endfunction()

function(add_resources
    files)

    target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${files})
if(APPLE)
    foreach(element IN LISTS files)
        list(APPEND res_files ${CMAKE_CURRENT_SOURCE_DIR}/${element})
    endforeach()
    
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
                        FRAMEWORK TRUE
                        MACOSX_BUNDLE TRUE
                        MACOSX_FRAMEWORK_IDENTIFIER org.cmake.synthesis
                        RESOURCE "${res_files}")
endif()
if(UNIX AND NOT APPLE)
    foreach(element IN LISTS files)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${element} ${CMAKE_BINARY_DIR}/resources COPYONLY)
    endforeach()
endif()
if(WIN32)
    foreach(element IN LISTS files)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${element} ${CMAKE_BINARY_DIR}/resources COPYONLY)
    endforeach()
endif()

endfunction()
