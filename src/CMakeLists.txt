add_executable(hdf2vis "main.cpp")
target_link_libraries(hdf2vis ${HDF5_LIBRARIES} ${PNG_LIBRARY} ${LIBPNG_LIBRARIES} fmt)
