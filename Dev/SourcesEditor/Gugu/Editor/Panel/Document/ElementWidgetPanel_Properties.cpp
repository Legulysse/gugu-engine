////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ElementWidgetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

void ElementWidgetPanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
    if (!m_selectedElementData)
        return;

    ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

    Element* element = m_selectedElement;
    if (ImGui::CollapsingHeader("Common##_HEADER", headerFlags))
    {
        // Name
        ImGui::InputText("Name", &m_selectedElementData->name);
        ImGui::Spacing();

        ElementData* elementData = dynamic_cast<ElementData*>(m_selectedElementData);
        if (elementData)
        {
            // Origin
            ImGui::BeginDisabled(elementData->useDimOrigin);
            if (ImGui::InputFloat2("Origin", &elementData->origin))
            {
                element->SetOrigin(elementData->origin);
                RaiseDirty();
            }
            ImGui::EndDisabled();

            if (ImGui::Checkbox("Use Unified Origin", &elementData->useDimOrigin))
            {
                if (elementData->useDimOrigin)
                {
                    elementData->dimOrigin = UDim2(Vector2::Zero_f, elementData->origin);
                    elementData->origin = Vector2::Zero_f;
                    element->SetUnifiedOrigin(elementData->dimOrigin);
                    RaiseDirty();
                }
                else
                {
                    elementData->origin = elementData->dimOrigin.absolute;
                    elementData->dimOrigin = UDim2::ZERO;
                    element->ResetUnifiedOrigin();
                    RaiseDirty();
                }
            }

            ImGui::BeginDisabled(!elementData->useDimOrigin);
            if (ImGui::InputFloat4("Unified Origin", &elementData->dimOrigin))
            {
                element->SetUnifiedOrigin(elementData->dimOrigin);
                RaiseDirty();
            }
            ImGui::EndDisabled();

            ImGui::Spacing();

            // Position
            ImGui::BeginDisabled(elementData->useDimPosition);
            if (ImGui::InputFloat2("Position", &elementData->position))
            {
                element->SetPosition(elementData->position);
                RaiseDirty();
            }
            ImGui::EndDisabled();

            if (ImGui::Checkbox("Use Unified Position", &elementData->useDimPosition))
            {
                if (elementData->useDimPosition)
                {
                    elementData->dimPosition = UDim2(Vector2::Zero_f, elementData->position);
                    elementData->position = Vector2::Zero_f;
                    element->SetUnifiedPosition(elementData->dimPosition);
                    RaiseDirty();
                }
                else
                {
                    elementData->position = elementData->dimPosition.absolute;
                    elementData->dimPosition = UDim2::ZERO;
                    element->ResetUnifiedPosition();
                    RaiseDirty();
                }
            }

            ImGui::BeginDisabled(!elementData->useDimPosition);
            if (ImGui::InputFloat4("Unified Position", &elementData->dimPosition))
            {
                element->SetUnifiedPosition(elementData->dimPosition);
                RaiseDirty();
            }
            ImGui::EndDisabled();

            ImGui::Spacing();

            // Size
            ImGui::BeginDisabled(elementData->useDimSize);
            if (ImGui::InputFloat2("Size", &elementData->size))
            {
                element->SetSize(elementData->size);
                RaiseDirty();
            }
            ImGui::EndDisabled();

            if (ImGui::Checkbox("Use Unified Size", &elementData->useDimSize))
            {
                if (elementData->useDimSize)
                {
                    elementData->dimSize = UDim2(Vector2::Zero_f, elementData->size);
                    elementData->size = Vector2::Zero_f;
                    element->SetUnifiedSize(elementData->dimSize);
                    RaiseDirty();
                }
                else
                {
                    elementData->size = elementData->dimSize.absolute;
                    elementData->dimSize = UDim2::ZERO;
                    element->ResetUnifiedSize();
                    RaiseDirty();
                }
            }

            ImGui::BeginDisabled(!elementData->useDimSize);
            if (ImGui::InputFloat4("Unified Size", &elementData->dimSize))
            {
                element->SetUnifiedSize(elementData->dimSize);
                RaiseDirty();
            }
            ImGui::EndDisabled();

            ImGui::Spacing();

            // Rotation
            if (ImGui::InputFloat("Rotation", &elementData->rotation))
            {
                element->SetRotation(elementData->rotation);
                RaiseDirty();
            }

            // Flip
            if (ImGui::Checkbox("Flip V", &elementData->flipV))
            {
                element->SetFlipV(elementData->flipV);
                RaiseDirty();
            }

            if (ImGui::Checkbox("Flip H", &elementData->flipH))
            {
                element->SetFlipH(elementData->flipH);
                RaiseDirty();
            }
        }
    }

    ElementSpriteGroupData* elementSpriteGroupData = dynamic_cast<ElementSpriteGroupData*>(m_selectedElementData);
    ElementSpriteGroup* elementSpriteGroup = dynamic_cast<ElementSpriteGroup*>(element);
    if (elementSpriteGroupData && ImGui::CollapsingHeader("Sprite Group##_HEADER", headerFlags))
    {
        Texture* texture = elementSpriteGroupData->texture;
        std::string textureId = !texture ? "" : texture->GetID();
        if (ImGui::InputText("Texture", &textureId, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            elementSpriteGroupData->texture = GetResources()->GetTexture(textureId);
            elementSpriteGroupData->imageSet = nullptr;
            elementSpriteGroup->SetTexture(elementSpriteGroupData->texture);
            RaiseDirty();
        }

        ImageSet* imageSet = elementSpriteGroupData->imageSet;
        std::string imageSetId = !imageSet ? "" : imageSet->GetID();
        if (ImGui::InputText("ImageSet", &imageSetId, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            elementSpriteGroupData->imageSet = GetResources()->GetImageSet(imageSetId);
            elementSpriteGroupData->texture = nullptr;
            elementSpriteGroup->SetTexture(!elementSpriteGroupData->imageSet ? nullptr : elementSpriteGroupData->imageSet->GetTexture());
            RaiseDirty();
        }

        DisplayGenerators(elementSpriteGroupData, elementSpriteGroup);
    }

    ElementSpriteBaseData* elementSpriteBaseData = dynamic_cast<ElementSpriteBaseData*>(m_selectedElementData);
    ElementSpriteBase* elementSpriteBase = dynamic_cast<ElementSpriteBase*>(element);
    if (elementSpriteBaseData && ImGui::CollapsingHeader("Sprite##_HEADER", headerFlags))
    {
        ElementSpriteData* elementSpriteData = dynamic_cast<ElementSpriteData*>(m_selectedElementData);
        ElementSprite* elementSprite = dynamic_cast<ElementSprite*>(element);
        ElementSpriteGroupItemData* elementSpriteGroupItemData = dynamic_cast<ElementSpriteGroupItemData*>(m_selectedElementData);
        ElementSpriteGroupItem* elementSpriteGroupItem = dynamic_cast<ElementSpriteGroupItem*>(element);

        if (elementSpriteData)
        {
            Texture* texture = elementSpriteData->texture;
            std::string textureId = !texture ? "" : texture->GetID();
            if (ImGui::InputText("Texture", &textureId, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                elementSpriteData->texture = GetResources()->GetTexture(textureId);
                elementSpriteData->imageSet = nullptr;
                elementSpriteData->subImageName = "";
                elementSprite->SetTexture(elementSpriteData->texture
                    , texture == nullptr || elementSpriteData->texture == nullptr   // Only update rect if texture was null, or set to null.
                    , texture == nullptr);                                          // Only update size if texture was null.
                elementSpriteData->textureRect = elementSprite->GetSubRect();
                elementSpriteData->size = elementSprite->GetSize();
                RaiseDirty();
            }

            ImageSet* imageSet = elementSpriteData->imageSet;
            std::string imageSetId = !imageSet ? "" : imageSet->GetID();
            if (ImGui::InputText("ImageSet", &imageSetId, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                elementSpriteData->imageSet = GetResources()->GetImageSet(imageSetId);
                elementSpriteData->texture = nullptr;
                elementSpriteData->subImageName = "";
                elementSprite->SetTexture(!elementSpriteData->imageSet ? nullptr : elementSpriteData->imageSet->GetTexture(), false);
                elementSpriteData->textureRect = elementSprite->GetSubRect();
                elementSpriteData->size = elementSprite->GetSize();
                RaiseDirty();
            }

            if (ImGui::InputText("SubImage", &elementSpriteData->subImageName, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                SubImage* subImage = elementSpriteData->imageSet->GetSubImage(elementSpriteData->subImageName);
                elementSpriteGroupItem->SetSubRect(!subImage ? sf::IntRect() : subImage->GetRect(), false);
                elementSpriteData->textureRect = sf::IntRect();
                RaiseDirty();
            }

            if (ImGui::InputInt4("TextureRect", &elementSpriteData->textureRect))
            {
                elementSpriteBase->SetSubRect(elementSpriteData->textureRect, false);
                elementSpriteData->subImageName = "";
                RaiseDirty();
            }
        }
        else if (elementSpriteGroupItemData)
        {
            ElementSpriteGroup* ownerGroup = dynamic_cast<ElementSpriteGroup*>(elementSpriteGroupItem->GetParent());
            ElementSpriteGroupData* ownerGroupData = dynamic_cast<ElementSpriteGroupData*>(m_dataBindings->dataFromElement.at(ownerGroup));

            if (ImGui::InputText("SubImage", &elementSpriteGroupItemData->subImageName, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                SubImage* subImage = ownerGroupData->imageSet->GetSubImage(elementSpriteGroupItemData->subImageName);
                elementSpriteGroupItem->SetSubRect(!subImage ? sf::IntRect() : subImage->GetRect(), false);
                elementSpriteGroupItemData->textureRect = sf::IntRect();
                RaiseDirty();
            }

            if (ImGui::InputInt4("TextureRect", &elementSpriteGroupItemData->textureRect))
            {
                elementSpriteGroupItem->SetSubRect(elementSpriteGroupItemData->textureRect, false);
                elementSpriteGroupItemData->subImageName = "";
                RaiseDirty();
            }
        }

        if (ImGui::Checkbox("Repeat Texture", &elementSpriteBaseData->repeatTexture))
        {
            elementSpriteBase->SetRepeatTexture(elementSpriteBaseData->repeatTexture);
            RaiseDirty();
        }

        if (ImGui::Checkbox("Flip Texture V", &elementSpriteBaseData->flipTextureV))
        {
            elementSpriteBase->SetFlipTextureV(elementSpriteBaseData->flipTextureV);
            RaiseDirty();
        }

        if (ImGui::Checkbox("Flip Texture H", &elementSpriteBaseData->flipTextureH))
        {
            elementSpriteBase->SetFlipTextureH(elementSpriteBaseData->flipTextureH);
            RaiseDirty();
        }

        if (ImGui::ColorEdit4("Color", &elementSpriteBaseData->color))
        {
            elementSpriteBase->SetColor(elementSpriteBaseData->color);
            RaiseDirty();
        }
    }

    ElementTextData* elementTextData = dynamic_cast<ElementTextData*>(m_selectedElementData);
    ElementText* elementText = dynamic_cast<ElementText*>(element);
    if (elementTextData && ImGui::CollapsingHeader("Text##_HEADER", headerFlags))
    {
        if (ImGui::InputText("Text", &elementTextData->text, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            // Assume Utf8 data (same as inside ElementText::LoadFromData).
            elementText->SetText(sf::String::fromUtf8(elementTextData->text.begin(), elementTextData->text.end()));
            elementTextData->size = elementText->GetSize();
            RaiseDirty();
        }

        Font* font = elementTextData->font;
        std::string fontId = !font ? "" : font->GetID();
        if (ImGui::InputText("Font", &fontId, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            elementTextData->font = GetResources()->GetFont(fontId);
            elementText->SetFont(elementTextData->font);
            elementTextData->size = elementText->GetSize();
            RaiseDirty();
        }

        if (ImGui::Checkbox("Multiline", &elementTextData->multiline))
        {
            elementText->SetMultiline(elementTextData->multiline);
            elementTextData->size = elementText->GetSize();
            RaiseDirty();
        }

        static const std::vector<std::string> resizeRules = { "FixedSize", "FitSize", "FitHeight", "FitScale" };

        size_t resizeRuleIndex = (size_t)elementTextData->resizeRule;
        if (ImGui::Combo("Resize Rule", resizeRules, &resizeRuleIndex))
        {
            elementTextData->resizeRule = (ETextResizeRule::Type)resizeRuleIndex;
            elementText->SetResizeRule(elementTextData->resizeRule);
            elementTextData->size = elementText->GetSize();
            RaiseDirty();
        }
    }
}

void ElementWidgetPanel::DisplayGenerators(ElementSpriteGroupData* elementSpriteGroupData, ElementSpriteGroup* elementSpriteGroup)
{
    Texture* texture = elementSpriteGroupData->texture;
    ImageSet* imageSet = elementSpriteGroupData->imageSet;

    ImGui::Spacing();

    static const std::vector<std::string> generators = { "...", "Box9 - SubImages", "Box9 - Texture Rects" };
    ImGui::Combo("Generator", generators, &m_generatorIndex);

    ImGui::Spacing();

    bool generateDimensions = false;

    if (m_generatorIndex == 1)
    {
        if (!imageSet)
        {
            ImGui::Text("Please select an ImageSet");
        }
        else
        {
            // Generator : SubImages.
            ImGui::PushID("_SUBIMAGE_NAMES");

            ImGui::InputText("Top Left", &m_topLeftName);
            ImGui::InputText("Top", &m_topName);
            ImGui::InputText("Top Right", &m_topRightName);

            ImGui::InputText("Left", &m_leftName);
            ImGui::InputText("Center", &m_centerName);
            ImGui::InputText("Right", &m_rightName);

            ImGui::InputText("Bottom Left", &m_bottomLeftName);
            ImGui::InputText("Bottom", &m_bottomName);
            ImGui::InputText("Bottom Right", &m_bottomRightName);

            ImGui::Spacing();

            ImGui::Checkbox("Tile Top", &m_tileTopSegment);
            ImGui::Checkbox("Tile Left", &m_tileLeftSegment);
            ImGui::Checkbox("Tile Right", &m_tileRightSegment);
            ImGui::Checkbox("Tile Bottom", &m_tileBottomSegment);

            ImGui::Spacing();

            if (ImGui::Button("Generate"))
            {
                std::vector<std::string> generatorSubImageNames = { m_topLeftName, m_topName, m_topRightName, m_leftName, m_centerName, m_rightName, m_bottomLeftName, m_bottomName, m_bottomRightName };
                std::vector<SubImage*> generatorSubImages;

                for (size_t i = 0; i < generatorSubImageNames.size(); ++i)
                {
                    generatorSubImages.push_back(imageSet->GetSubImage(generatorSubImageNames[i]));
                }

                if (generatorSubImages.size() == generatorSubImageNames.size())
                {
                    for (size_t i = 0; i < generatorSubImageNames.size(); ++i)
                    {
                        ElementSpriteGroupItemData* componentData = new ElementSpriteGroupItemData;
                        ElementSpriteGroupItem* component = AppendNewComponent(elementSpriteGroupData, componentData);

                        componentData->subImageName = generatorSubImageNames[i];
                        componentData->textureRect = sf::IntRect();
                        component->SetSubRect(generatorSubImages[i]->GetRect(), true);
                        componentData->size = component->GetSize();
                    }

                    generateDimensions = true;
                }
            }

            ImGui::PopID();
        }
    }
    else if (m_generatorIndex == 2)
    {
        if (!texture)
        {
            ImGui::Text("Please select a Texture");
        }
        else
        {
            // Generator : Texture Rects.
            ImGui::PushID("_TEXTURE_RECTS");

            ImGui::InputInt4("Top Left", &m_topLeftRect);
            ImGui::InputInt4("Top", &m_topRect);
            ImGui::InputInt4("Top Right", &m_topRightRect);

            ImGui::InputInt4("Left", &m_leftRect);
            ImGui::InputInt4("Center", &m_centerRect);
            ImGui::InputInt4("Right", &m_rightRect);

            ImGui::InputInt4("Bottom Left", &m_bottomLeftRect);
            ImGui::InputInt4("Bottom", &m_bottomRect);
            ImGui::InputInt4("Bottom Right", &m_bottomRightRect);

            ImGui::Spacing();

            ImGui::Checkbox("Tile Top", &m_tileTopSegment);
            ImGui::Checkbox("Tile Left", &m_tileLeftSegment);
            ImGui::Checkbox("Tile Right", &m_tileRightSegment);
            ImGui::Checkbox("Tile Bottom", &m_tileBottomSegment);

            ImGui::Spacing();

            if (ImGui::Button("Generate"))
            {
                std::vector<sf::IntRect> generatorTextureRects = { m_topLeftRect, m_topRect, m_topRightRect, m_leftRect, m_centerRect, m_rightRect, m_bottomLeftRect, m_bottomRect, m_bottomRightRect };

                for (size_t i = 0; i < generatorTextureRects.size(); ++i)
                {
                    ElementSpriteGroupItemData* componentData = new ElementSpriteGroupItemData;
                    ElementSpriteGroupItem* component = AppendNewComponent(elementSpriteGroupData, componentData);

                    componentData->subImageName = "";
                    componentData->textureRect = generatorTextureRects[i];
                    component->SetSubRect(generatorTextureRects[i], true);
                    componentData->size = component->GetSize();
                }

                generateDimensions = true;
            }

            ImGui::PopID();
        }
    }

    if (generateDimensions)
    {
        auto unifiedDimensionsGenerator = [](ElementSpriteGroupItem* component, ElementSpriteGroupItemData* componentData, UDim2 position, UDim2 size)
        {
            componentData->useDimPosition = true;
            componentData->dimPosition = position;
            component->SetUnifiedPosition(position);

            componentData->useDimSize = true;
            componentData->dimSize = size;
            component->SetUnifiedSize(size);
        };

        const std::vector<ElementSpriteGroupItem*>& componentElements = elementSpriteGroup->GetItems();

        // Top left.
        unifiedDimensionsGenerator(componentElements[0], elementSpriteGroupData->components[0]
            , UDim2::POSITION_TOP_LEFT
            , UDim2::SIZE_ZERO + elementSpriteGroupData->components[0]->size);

        // Top center.
        unifiedDimensionsGenerator(componentElements[1], elementSpriteGroupData->components[1]
            , UDim2::POSITION_TOP_LEFT + Vector2f(elementSpriteGroupData->components[0]->size.x, 0.f)
            , UDim2(1.f, 0.f) + Vector2f(-elementSpriteGroupData->components[0]->size.x - elementSpriteGroupData->components[2]->size.x, elementSpriteGroupData->components[1]->size.y));

        // Top right.
        unifiedDimensionsGenerator(componentElements[2], elementSpriteGroupData->components[2]
            , UDim2::POSITION_TOP_RIGHT + Vector2f(-elementSpriteGroupData->components[2]->size.x, 0.f)
            , UDim2::SIZE_ZERO + elementSpriteGroupData->components[2]->size);

        // Center left.
        unifiedDimensionsGenerator(componentElements[3], elementSpriteGroupData->components[3]
            , UDim2::POSITION_TOP_LEFT + Vector2f(0.f, elementSpriteGroupData->components[0]->size.y)
            , UDim2(0.f, 1.f) + Vector2f(elementSpriteGroupData->components[3]->size.x, -elementSpriteGroupData->components[0]->size.y - elementSpriteGroupData->components[6]->size.y));

        // Center.
        unifiedDimensionsGenerator(componentElements[4], elementSpriteGroupData->components[4]
            , UDim2::POSITION_TOP_LEFT + Vector2f(elementSpriteGroupData->components[3]->size.x, elementSpriteGroupData->components[1]->size.y)
            , UDim2::SIZE_FULL + Vector2f(-elementSpriteGroupData->components[3]->size.x - elementSpriteGroupData->components[5]->size.x, -elementSpriteGroupData->components[1]->size.y - elementSpriteGroupData->components[7]->size.y));

        // Center right.
        unifiedDimensionsGenerator(componentElements[5], elementSpriteGroupData->components[5]
            , UDim2::POSITION_TOP_RIGHT + Vector2f(-elementSpriteGroupData->components[5]->size.x, elementSpriteGroupData->components[2]->size.y)
            , UDim2(0.f, 1.f) + Vector2f(elementSpriteGroupData->components[5]->size.x, -elementSpriteGroupData->components[2]->size.y - elementSpriteGroupData->components[8]->size.y));

        // Bottom left.
        unifiedDimensionsGenerator(componentElements[6], elementSpriteGroupData->components[6]
            , UDim2::POSITION_BOTTOM_LEFT + Vector2f(0.f, -elementSpriteGroupData->components[6]->size.y)
            , UDim2::SIZE_ZERO + elementSpriteGroupData->components[6]->size);

        // Bottom center.
        unifiedDimensionsGenerator(componentElements[7], elementSpriteGroupData->components[7]
            , UDim2::POSITION_BOTTOM_LEFT + Vector2f(elementSpriteGroupData->components[6]->size.x, -elementSpriteGroupData->components[7]->size.y)
            , UDim2(1.f, 0.f) + Vector2f(-elementSpriteGroupData->components[6]->size.x - elementSpriteGroupData->components[8]->size.x, elementSpriteGroupData->components[7]->size.y));

        // Bottom right.
        unifiedDimensionsGenerator(componentElements[8], elementSpriteGroupData->components[8]
            , UDim2::POSITION_BOTTOM_RIGHT + -elementSpriteGroupData->components[8]->size
            , UDim2::SIZE_ZERO + elementSpriteGroupData->components[8]->size);

        // Segments tiling.
        elementSpriteGroupData->components[1]->repeatTexture = m_tileTopSegment;
        componentElements[1]->SetRepeatTexture(m_tileTopSegment);

        elementSpriteGroupData->components[3]->repeatTexture = m_tileLeftSegment;
        componentElements[3]->SetRepeatTexture(m_tileLeftSegment);

        elementSpriteGroupData->components[5]->repeatTexture = m_tileRightSegment;
        componentElements[5]->SetRepeatTexture(m_tileRightSegment);

        elementSpriteGroupData->components[7]->repeatTexture = m_tileBottomSegment;
        componentElements[7]->SetRepeatTexture(m_tileBottomSegment);

        // Finalize.
        RaiseDirty();
    }
}

}   //namespace gugu
