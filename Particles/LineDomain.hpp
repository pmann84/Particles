#pragma once

#include "IDomain.hpp"

namespace ps
{
    class LineDomain : public IDomain
    {
    public:
        LineDomain(Vector3d begin, Vector3d end);
        ~LineDomain() = default;

        Vector3d GeneratePosition() override;
        Vector3d GenerateVelocity(Vector3d start_position, double speed) override;
        bool HasEnteredDomain(const Particle &particle) const override;
        Vector3d Intersection(const Particle &particle) const override;
        Vector3d Normal() override;
        void Draw(IDomainRenderer &draw_interface) override;

        Vector3d StartPoint() const { return m_begin; }
        Vector3d EndPoint() const { return m_end; }

    private:
        Vector3d m_begin;
        Vector3d m_end;
    };
}