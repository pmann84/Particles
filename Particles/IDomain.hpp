#pragma once

#include "Particle.hpp"
#include "IDomainRenderer.hpp"

namespace ps
{
    class IDomain
    {
    public:
        virtual ~IDomain() = default;
        virtual Vector3d GeneratePosition() = 0;
        virtual Vector3d GenerateVelocity(Vector3d startPosition, double speed) = 0;
        [[nodiscard]] virtual Vector3d Intersection(const Particle& particle) const = 0;
        [[nodiscard]] virtual bool HasEnteredDomain(const Particle& particle) const = 0;
        virtual Vector3d Normal() = 0;
        virtual void Draw(IDomainRenderer& renderer) = 0;
    };
}