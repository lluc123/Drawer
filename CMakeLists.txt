project(waveview)
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
