project(waveview)

if(WIN32)
set(includeSDL C:/usr/include/)
set(linkSDL C:/usr/lib/)
else()
set(includeSDL /usr/include/)
set(linkSDL /usr/lib/)
endif()


include_directories(
${includeSDL}
)

link_directories(${linkSDL})

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
