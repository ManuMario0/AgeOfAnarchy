set(INC
    .
    intern
    headers
)

set(SRC
    intern/list.c
)

set(LIB
)

add_module(common "${SRC}" "${INC}" "${LIB}")
