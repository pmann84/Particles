#define _USE_MATH_DEFINES
#include <cmath>

#include <ApplicationBase.hpp>
#include <ParticleGroup.hpp>
#include <DiskDomain.hpp>

#include <imgui.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <sstream>

glm::vec4 DefaultColour = glm::vec4(0.3, 0.7, 0.4, 1.0);

class BasicApp : public gfxlib::ApplicationBase {
public:
    explicit BasicApp(gfxlib::WindowSettings settings) : gfxlib::ApplicationBase(settings)
        , m_particleSystem(750, 5)
    {
        m_particleSystem.time_interval(0.05);
        m_particleSystem.particle_life(150, true, 6);
    }

    void OnUpdate(float ts) override
    {
        glClearColor(DefaultColour.r, DefaultColour.g, DefaultColour.b, DefaultColour.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //std::shared_ptr<ps::IDomain> source_domain = std::make_shared<ps::point_domain>(ps::Vector3d(0.0f, 0.0f, 0.0f));
        //std::shared_ptr<ps::IDomain> source_domain = std::make_shared<ps::line_domain>(ps::Vector3d(0.0f, 0.0f, -5.0f), ps::vector3d(-0.1f, 0.0f, 0.0f));
        std::shared_ptr<ps::IDomain> srcDomain = std::make_shared<ps::DiskDomain>(ps::Vector3d(0.0f, 0.0f, 0.0f), 0.1, 0.25, M_PI / 2, M_PI / 2);

        std::shared_ptr<ps::IDomain> velocityDomain = std::make_shared<ps::DiskDomain>(ps::Vector3d(0.0f, 5.0f, 0.0f), 0.75, 1.5, M_PI/2, M_PI/2);
        //std::shared_ptr<ps::IDomain> velocity_domain = std::make_shared<ps::line_domain>(ps::Vector3d(-0.1, 1.0f, 0.0f), ps::vector3d(0.1f, 1.0f, 0.0f));
        //std::shared_ptr<ps::IDomain> velocity_domain = std::make_shared<ps::point_domain>(ps::Vector3d(3.0f, 3.0f, 0.0f));

        std::shared_ptr<ps::IDomain> sinkDomain = std::make_shared<ps::DiskDomain>(ps::Vector3d(0.0f, -1.0f, 0.0f), 0.0, 5.0, M_PI / 2, M_PI / 2);
        std::shared_ptr<ps::IDomain> bounceDomain = std::make_shared<ps::DiskDomain>(ps::Vector3d(0.0f, -0.5f, 0.0f), 0.0, 2.0, M_PI / 2, M_PI / 2);

        ps::Vector3d gravity_vector(0.0, -2.0, 0.0);
        //float random_vel_range = 5.0;
        //ps::vector3d wind_vector(ps::generate_random_number_in_range(-random_vel_range, random_vel_range),
        //						 ps::generate_random_number_in_range(-random_vel_range, random_vel_range),
        //						 ps::generate_random_number_in_range(-random_vel_range, random_vel_range));

        m_particleSystem.source(srcDomain)
                .bounce(bounceDomain, 0.1)
                .sink(sinkDomain)
                .velocity(velocityDomain, 3, true, 4)
                .constant_force(gravity_vector)
                .update();

//        for(auto& p: m_particleSystem.particles())
//        {
//            if (!p.is_dead())
//            {
//                // TODO: Fix this not applying the colour!
//                glColor3f(static_cast<float>(p.colour()[0]), static_cast<float>(p.colour()[1]), static_cast<float>(p.colour()[2]));
//                glVertex3f(p.position()[0], p.position()[1], p.position()[2]);
//            }
//        }
//        glEnd();
//        //// DEBUG: Render particle velocity vectors
//        //for (auto& p : p_group.particles())
//        //{
//        //   if (!p.is_dead() && !p.is_static())
//        //   {
//        //      glBegin(GL_LINES);
//        //      glVertex3f(p.position()[0], p.position()[1], p.position()[2]);
//        //      ps::vector3d end_pt = p.position() + 0.2 * p.velocity();
//        //      glVertex3f(end_pt[0], end_pt[1], end_pt[2]);
//        //      glEnd();
//        //   }
//        //}
//        p_group.draw_domains(m_domain_drawer);
    }

    void OnUiRender() override
    {
        ImGui::Begin("Hardware Info");
        ImGui::Text("Vendor: %s", m_context.info().vendor.c_str());
        ImGui::Text("Renderer: %s", m_context.info().renderer.c_str());
        ImGui::Text("Version: %s", m_context.info().version.c_str());
        ImGui::End();
    }

    void OnWindowResize(const WindowResizeEvent& event) override {
        std::cout << "Window resize called: " << event.width << " x " << event.height << std::endl;
    }

    void OnKeyPressed(const KeyPressedEvent& event) override {
        std::cout << "Key pressed..." << event.key << std::endl;
    }

    std::string getInfo() {
        auto ctxInfo = m_context.info();
        std::stringstream ss;
        ss << "Vendor: " << ctxInfo.vendor << std::endl << "Renderer: " << ctxInfo.renderer << std::endl << "Version: " << ctxInfo.version << std::endl;
        return ss.str();
    }

private:
    ps::ParticleGroup m_particleSystem;
};

int main()
{
    gfxlib::WindowSettings settings;
    settings.width = 800;
    settings.height = 600;
    settings.title = "OpenGL Basic App";

    auto myApp = BasicApp(settings);
    myApp.start();

    return 0;
}