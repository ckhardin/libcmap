add_library(cmap
    orderedmap.c
    orderedmap_types.c
)
target_code_coverage(cmap AUTO)

target_include_directories(cmap
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
