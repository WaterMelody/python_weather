set(SRCS
	evaluation.cpp
)

add_source_groups(${SRCS})

add_library(evaluation STATIC ${SRCS})

target_link_libraries(evaluation
	libboost_serialization.a
	libboost_filesystem.a
)
