#pragma once

namespace ps
{
    class PointDomain;
    class LineDomain;
    class DiskDomain;

    class IDomainRenderer
    {
    public:
        virtual ~IDomainRenderer() = default;
        virtual void Draw(PointDomain* domain) = 0;
        virtual void Draw(LineDomain* domain) = 0;
        virtual void Draw(DiskDomain* domain) = 0;
    };
}
