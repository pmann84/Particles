#pragma once

#include "IDomain.hpp"

namespace ps
{
    // Disk Domain - zenith = M_PI/2 and azimuth = M_PI/2, give a disk that is oriented in the x, z plane in openGL.
    class DiskDomain : public IDomain
    {
    public:
        DiskDomain(Vector3d
        centre,
        double inner_radius,
        double outer_radius,
        double zenith,
        double azimuth
        );

        ~DiskDomain() = default;

        Vector3d GeneratePosition() override;
        Vector3d GenerateVelocity(Vector3d start_position, double speed) override;

        bool HasEnteredDomain(const Particle& particle) const override;
        Vector3d Intersection(const Particle& particle) const override;
        Vector3d Normal() override;

        void Draw(IDomainRenderer& draw_interface) override;

        Vector3d Centre() const { return m_disc_centre; }
        double InnerRadius() const { return m_inner_radius; }
        double OuterRadius() const { return m_outer_radius; }

    private:
        Vector3d GeneratePosition(double radius, double angle) const;

    private:
        Vector3d m_disc_centre;
        double m_inner_radius, m_outer_radius;
        double m_theta, m_phi;
    };
}