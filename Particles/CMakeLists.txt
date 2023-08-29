set(LIB_PROJECT_NAME Particles)
message(STATUS "Setting up project for app ${LIB_PROJECT_NAME}")

add_library(${LIB_PROJECT_NAME})

target_sources(${LIB_PROJECT_NAME}
        PRIVATE
            Vector.hpp
            Particle.cpp Particle.hpp
            ParticleGroup.cpp ParticleGroup.hpp
            IDomain.hpp
            IDomainRenderer.hpp
            DiskDomain.cpp DiskDomain.hpp
            LineDomain.cpp LineDomain.hpp
            PointDomain.cpp PointDomain.hpp
            TriangleDomain.cpp TriangleDomain.hpp
            Random.cpp Random.hpp
)

# Exports all the include directories needed for this library
target_include_directories(${LIB_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})