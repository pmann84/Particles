#include "TriangleDomain.hpp"

namespace ps
{
    TriangleDomain::TriangleDomain(Vector3d p1, Vector3d p2, Vector3d p3) : m_p1(p1), m_p2(p2), m_p3(p3)
    {
    }

    Vector3d TriangleDomain::GeneratePosition()
    {
        return Vector3d();
    }

    Vector3d TriangleDomain::GenerateVelocity(Vector3d start_position, double speed)
    {
        return Vector3d();
    }

    Vector3d TriangleDomain::Intersection(const Particle &particle) const
    {
        return Vector3d();
    }

    bool TriangleDomain::HasEnteredDomain(const Particle& particle) const
    {
        return false;
    }

    Vector3d TriangleDomain::Normal()
    {
        return Vector3d();
    }

    void TriangleDomain::Draw(IDomainRenderer &draw_interface)
    {
    }
}