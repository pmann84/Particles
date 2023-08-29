#include "PointDomain.hpp"

namespace ps
{
    PointDomain::PointDomain(Vector3d point) : IDomain(), m_point(point)
    {
    }

    Vector3d PointDomain::GeneratePosition()
    {
        return m_point;
    }

    Vector3d PointDomain::GenerateVelocity(Vector3d start_position, double speed)
    {
        Vector3d velocity_vector(m_point - start_position);
        velocity_vector.normalise();
        velocity_vector *= speed;
        return velocity_vector;
    }

    bool PointDomain::HasEnteredDomain(const Particle& particle) const
    {
        // c is point from domain a and b are start and end of line
        // need to return distance(A, C) + distance(B, C) == distance(A, B)
        Vector3d start_point = particle.previous_position();
        Vector3d end_point = particle.position();
        return start_point.distance(m_point) + end_point.distance(m_point) == start_point.distance(end_point);
    }

    Vector3d PointDomain::Intersection(const Particle& particle) const
    {
        // TODO: Implement intersection calculation here
        return m_point;
    }

    Vector3d PointDomain::Normal()
    {
        // TODO: Implement normal calculation here
        return Vector3d();
    }

    void PointDomain::Draw(IDomainRenderer& renderer)
    {
        renderer.Draw(this);
    }
}