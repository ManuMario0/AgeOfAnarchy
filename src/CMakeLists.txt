# -----------------------------------
# - main sources and dependencies   -
# -----------------------------------

target_sources(${CMAKE_PROJECT_NAME} PUBLIC main.c)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VERSION ${CMAKE_PROJECT_VERSION})

# -----------------------------------
# - subdirectories                  -
# -----------------------------------

add_subdirectory("memory")
add_subdirectory("common")
add_subdirectory("renderer")
add_subdirectory("core")
