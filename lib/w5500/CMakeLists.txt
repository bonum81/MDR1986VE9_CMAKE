add_library(w5500 STATIC
    src/w5500.c
    src/wizchip_conf.c
    src/dhcp.c
    src/loopback.c
    src/socket.c
)

target_include_directories(w5500 PUBLIC
    include
)
