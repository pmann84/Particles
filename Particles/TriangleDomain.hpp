#pragma once

#include "IDomain.hpp"

namespace ps
{
    class TriangleDomain : public IDomain
    {
    public:
        TriangleDomain(Vector3d p1, Vector3d p2, Vector3d p3);
        ~TriangleDomain() = default;

        Vector3d GeneratePosition() override;
        Vector3d GenerateVelocity(Vector3d start_position, double speed) override;
        Vector3d Intersection(const Particle &particle) const override;
        bool HasEnteredDomain(const Particle &particle) const override;
        Vector3d Normal() override;

        void Draw(IDomainRenderer &draw_interface) override;

    private:
        Vector3d m_p1, m_p2, m_p3;
    };
}