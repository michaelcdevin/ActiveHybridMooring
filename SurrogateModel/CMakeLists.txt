cmake_minimum_required(VERSION 3.10)
project(surrogate_model)

# Find Arrow and Parquet libraries
find_package(libKriging REQUIRED)

# Add the executable
add_executable(create_surrogate_models create_surrogate_models.cpp)

# Link the libraries
target_link_libraries(create_surrogate_models
	libKriging::Kriging
)

target_include_directories(parquet_armadillo_with_kriging
    ${LIBKRIGING_INCLUDE_DIRS}  # Add libKriging include directories
)