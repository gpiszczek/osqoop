set(VariousSinus_SRCS VariousSinus.cpp)
qt4_automoc(${VariousSinus_SRCS})
include_directories (${CMAKE_BINARY_DIR}/datasource/VariousSinus)
add_library(VariousSinus MODULE ${VariousSinus_SRCS})
install(TARGETS VariousSinus DESTINATION datasource)