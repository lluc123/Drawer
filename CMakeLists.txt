project(waveview)

include_directories(
/usr/include/
)

link_directories(/usr/lib/)

add_executable(
febonachi
waveview.c
)

target_link_libraries(
febonachi
m
)

target_link_libraries(
febonachi
SDL2
)
