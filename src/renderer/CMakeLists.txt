set(INC
    ../common
    .
)

set(SRC
    ./intern/init.c
    ./intern/command_buffer.c
    ./intern/renderer.c
    ./intern/model_loading.c
    ./intern/buffer.c
    ./intern/model.c
    ./intern/wavefront.c
    ./intern/object.c
    ./intern/camera.c
    ./intern/ui.c
    
    ./headers/window.h
    ./intern/init.h
    ./intern/command_buffer.h
    ./intern/renderer.h
    ./intern/model_loading.h
    ./intern/buffer.h
    ./intern/model.h
    ./intern/wavefront.h
    ./intern/object.h
    ./intern/camera.h
    ./intern/ui.h
)

set(LIB
)

add_module(renderer "${SRC}" "${INC}" "${LIB}")
