set(data_class_SRCS
  protein_sequence.cpp
  go_term.cpp
)

add_source_groups(${data_class_SRCS})

add_library(data_class STATIC ${data_class_SRCS})

target_link_libraries(data_class
libboost_serialization.a
libboost_filesystem.a
)

#target_include_directories(data_class PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})