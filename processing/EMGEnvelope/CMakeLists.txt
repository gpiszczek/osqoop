set(EMGEnvelope_SRCS EMGEnvelope.cpp)
qt4_automoc(${EMGEnvelope_SRCS})
include_directories (${CMAKE_BINARY_DIR}/processing/EMGEnvelope)
add_library(EMGEnvelope MODULE ${EMGEnvelope_SRCS})
target_link_libraries(EMGEnvelope processing)
install(TARGETS EMGEnvelope DESTINATION processing)