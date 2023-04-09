
set(INC
    ../renderer/headers
    ../common
    .
)

set(SRC
    intern/title_screen.c
    intern/world.c
    intern/sprite.c
    intern/loading.c
    intern/main_loop.c
    intern/player.c
    intern/context.c
    
    intern/title_screen.h
    intern/world.h
    intern/sprite.h
    intern/loading.h
    intern/main_loop.h
    intern/player.h
    intern/context.h
)

set(LIB
    renderer
)

add_module(core "${SRC}" "${INC}" "${LIB}")
