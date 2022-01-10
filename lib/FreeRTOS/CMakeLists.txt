add_library(freertos STATIC
    portable/GCC/ARM_CM3/port.c
    portable/MemMang/heap_4.c
    croutine.c
    event_groups.c
    list.c
    queue.c
    stream_buffer.c
    tasks.c
    timers.c
)

target_include_directories(freertos PUBLIC
    include
    portable/GCC/ARM_CM3/
)
