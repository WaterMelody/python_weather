set(SRCS
	select_data_set_main.cpp
)
add_source_groups(${SRCS})

add_executable(select_data_set_main ${SRCS})
target_link_libraries(select_data_set_main
	data_class
	libboost_serialization.a
	libboost_filesystem.a
)
