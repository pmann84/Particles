#pragma once

#include "IDomain.hpp"

namespace ps
{
    class PointDomain : public IDomain
    {
    public:
        PointDomain(Vector3d point);
        ~PointDomain() = default;

        Vector3d GeneratePosition() override;
        Vector3d GenerateVelocity(Vector3d start_position, double speed) override;

        bool HasEnteredDomain(const Particle& particle) const override;
        Vector3d Intersection(const Particle& particle) const override;
        Vector3d Normal() override;

        void Draw(IDomainRenderer& draw_interface) override;

        Vector3d point() const { return m_point; }

    private:
        Vector3d m_point;
    };
}