#pragma once

#include <exception>

namespace ps {
    class NoDomainIntersectionException : std::exception
    {
        virtual const char* what() const throw() override
        {
            return "Point does not intersect with domain.";
        }
    };

}