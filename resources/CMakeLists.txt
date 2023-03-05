# ------------------------
# - Shaders installation -
# ------------------------

set(RESOURCE_FILES
    shaders/vert.spv
    shaders/frag.spv
    models/sphere.3d
    models/sphere.mtl
    models/cube.3d
    models/cube.mtl
)

add_resources("${RESOURCE_FILES}")
