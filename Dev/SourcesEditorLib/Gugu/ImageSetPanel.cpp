////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/ImageSetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/RenderViewport.h"

#include "Gugu/Engine.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <imgui.h>
#include <imgui_internal.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ImageSetPanel::ImageSetPanel()
    : m_renderViewport(nullptr)
{
    m_renderViewport = new RenderViewport;

    ElementSprite* sprite = m_renderViewport->GetRoot()->AddChild<ElementSprite>();
    sprite->SetTexture("uipack_rpg.png");

    m_renderViewport->SetSize(Vector2u(sprite->GetSize()));
}

ImageSetPanel::~ImageSetPanel()
{
    SafeDelete(m_renderViewport);
}

void ImageSetPanel::Update(const DeltaTime& dt)
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;

    if (ImGui::Begin("ImageSet Editor", false, flags))
    {
        ImGui::Text("ImageSet panel goes here.");

        m_renderViewport->BeginRender();


        ImGuiIO& io = ImGui::GetIO();
        const ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
        const bool is_hovered = ImGui::IsItemHovered();
        const bool is_active = ImGui::IsItemActive();

        const ImVec2 origin(canvas_p0.x, canvas_p0.y);
        const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

        // TODO: check the drag start via ImGui::IsMouseClicked(ImGuiMouseButton_Left)
        static Vector2f itemPosition(0, 0);
        if (/*is_hovered && */ is_active && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            itemPosition = mouse_pos_in_canvas;
        }

        // TODO: Gizmo
        // - register a list of pickable gizmos
        // - method to pick an Element from a list of pickable elements, through the RenderViewport context
        // - click : pick and register as dragged Element
        // - is_active : move dragged Element depending on the offset with the starting position
        // - update coords every frame from the gizmo position

        sf::RectangleShape testShape;
        testShape.setOutlineThickness(1.f);
        testShape.setOutlineColor(sf::Color(255, 0, 0, 255));
        testShape.setFillColor(sf::Color(255, 0, 255, 100));
        testShape.setPosition(itemPosition);
        testShape.setSize(sf::Vector2f(50, 50));
        m_renderViewport->Render(&testShape);


        m_renderViewport->FinalizeRender();

#if 0
        static Vector2f itemPosition(0, 0);

        // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
        ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
        //ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
        //if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
        //if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
        Vector2u canvas_sz(1024, 512);

        //ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

        // Draw border and background color
        ImGuiIO& io = ImGui::GetIO();
        //ImDrawList* draw_list = ImGui::GetWindowDrawList();
        //draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
        //draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

        // This will catch our interactions
        ImGui::InvisibleButton("canvasInvisibleButton", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        const bool is_hovered = ImGui::IsItemHovered(); // Hovered
        const bool is_active = ImGui::IsItemActive();   // Held
        //const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
        //const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
        const ImVec2 origin(canvas_p0.x, canvas_p0.y); // Lock scrolled origin
        const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);


        // Little trick : draw an invisible button the size of the texture, then draw the texture all over it.
        ImGui::SetCursorScreenPos(canvas_p0);


        // TODO: check the drag start via ImGui::IsMouseClicked(ImGuiMouseButton_Left)
        if (/*is_hovered && */ is_active && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            itemPosition = mouse_pos_in_canvas;
        }


        if (m_renderTexture->getSize() != canvas_sz)
        {
            m_renderTexture->create(canvas_sz.x, canvas_sz.y);
        }

        m_renderTexture->clear(sf::Color(128, 128, 128, 255));

        sf::RectangleShape testShape;
        testShape.setOutlineThickness(1.f);
        testShape.setOutlineColor(sf::Color::Blue);
        testShape.setFillColor(sf::Color::Transparent);
        testShape.setPosition(itemPosition);
        testShape.setSize(sf::Vector2f(20, 20));

        m_renderTexture->draw(testShape);
#endif

        ImGui::Text("Now for some other tests.");

        ImGui::BeginChild("testtesttest", ImVec2(200, 100));

        for (int i = 0; i < 20; ++i)
        {
            ImGui::Text("Blah blah blah.");
            ImGui::Text("Bleh blih bloh.");
        }

        ImGui::EndChild();
    }
    ImGui::End();
}

}   //namespace gugu
