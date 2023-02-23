if(BUILD_TESTING)
    include(Catch)

    add_executable(test_orderedmap
        test_orderedmap.cpp
    )
    target_link_libraries(test_orderedmap
        PRIVATE
            cmap
            Catch2::Catch2WithMain
    )
    catch_discover_tests(test_orderedmap)
endif()
