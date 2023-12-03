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
    bool needRebuildHierarchy = false;

    if (ImGui::CollapsingHeader("Common##_HEADER", headerFlags))
    {
        // Name
        if (ImGui::InputText("Name", &m_selectedElementData->name))
        {
            RaiseDirty();
        }

        ImGui::Spacing();

        ElementWidgetInstanceData* widgetInstanceData = dynamic_cast<ElementWidgetInstanceData*>(m_selectedElementData);
        if (widgetInstanceData)
        {
            // Helpers
            int overrideId = 0;

            auto checkOverrideSetting = [](int& id, bool& useOverride) -> bool
            {
                ImGui::PushID(id);
                float posX_before = ImGui::GetCursorPosX();
                bool updated = ImGui::Checkbox("##Override", &useOverride);
                ImGui::PopID();

                ImGui::SameLine();

                float posX_after = ImGui::GetCursorPosX();
                ImGui::SetCursorPosX(posX_after - ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::SetNextItemWidth(ImGui::CalcItemWidth() + ImGui::GetStyle().ItemInnerSpacing.x - (posX_after - posX_before));

                ++id;
                return updated;
            };

            // Widget reference
            ElementWidget* widget = widgetInstanceData->widget;
            std::string widgetId = !widget ? "" : widget->GetID();
            if (ImGui::InputText("Widget", &widgetId, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                widgetInstanceData->widget = GetResources()->GetElementWidget(widgetId);

                RaiseDirty();
                needRebuildHierarchy = true;
            }

            ImGui::Spacing();

            if (widget)
            {
                // TODO: If the root data is a widget, I will need to iterate recursively on its own root data, until I find a usable root transform.
                ElementData* widgetRootData = dynamic_cast<ElementData*>(widget->GetRootData());

                // Origin
                if (checkOverrideSetting(overrideId, widgetInstanceData->overrideOrigin))
                {
                    if (!widgetInstanceData->overrideOrigin)
                    {
                        widgetInstanceData->dimOrigin = UDim2::ZERO;

                        if (widgetRootData->useDimOrigin)
                        {
                            element->SetUnifiedOrigin(widgetRootData->dimOrigin);
                        }
                        else
                        {
                            element->SetOrigin(widgetRootData->origin);
                        }
                    }
                    else
                    {
                        if (widgetRootData->useDimOrigin)
                        {
                            widgetInstanceData->dimOrigin = widgetRootData->dimOrigin;
                            element->SetUnifiedOrigin(widgetInstanceData->dimOrigin);
                        }
                        else
                        {
                            widgetInstanceData->dimOrigin = UDim2(Vector2::Zero_f, widgetRootData->origin);
                            element->SetUnifiedOrigin(widgetInstanceData->dimOrigin);
                        }
                    }

                    RaiseDirty();
                }

                ImGui::BeginDisabled(!widgetInstanceData->overrideOrigin);
                if (ImGui::InputFloat4("Unified Origin", &widgetInstanceData->dimOrigin))
                {
                    element->SetUnifiedOrigin(widgetInstanceData->dimOrigin);
                    RaiseDirty();
                }
                ImGui::EndDisabled();

                // Position
                if (checkOverrideSetting(overrideId, widgetInstanceData->overridePosition))
                {
                    if (!widgetInstanceData->overridePosition)
                    {
                        widgetInstanceData->dimPosition = UDim2::ZERO;

                        if (widgetRootData->useDimPosition)
                        {
                            element->SetUnifiedPosition(widgetRootData->dimPosition);
                        }
                        else
                        {
                            element->SetPosition(widgetRootData->position);
                        }
                    }
                    else
                    {
                        if (widgetRootData->useDimPosition)
                        {
                            widgetInstanceData->dimPosition = widgetRootData->dimPosition;
                            element->SetUnifiedPosition(widgetInstanceData->dimPosition);
                        }
                        else
                        {
                            widgetInstanceData->dimPosition = UDim2(Vector2::Zero_f, widgetRootData->position);
                            element->SetUnifiedPosition(widgetInstanceData->dimPosition);
                        }
                    }

                    RaiseDirty();
                }

                ImGui::BeginDisabled(!widgetInstanceData->overridePosition);
                if (ImGui::InputFloat4("Unified Position", &widgetInstanceData->dimPosition))
                {
                    element->SetUnifiedPosition(widgetInstanceData->dimPosition);
                    RaiseDirty();
                }
                ImGui::EndDisabled();

                // Size
                if (checkOverrideSetting(overrideId, widgetInstanceData->overrideSize))
                {
                    if (!widgetInstanceData->overrideSize)
                    {
                        widgetInstanceData->dimSize = UDim2::ZERO;

                        if (widgetRootData->useDimSize)
                        {
                            element->SetUnifiedSize(widgetRootData->dimSize);
                        }
                        else
                        {
                            element->SetSize(widgetRootData->size);
                        }
                    }
                    else
                    {
                        if (widgetRootData->useDimSize)
                        {
                            widgetInstanceData->dimSize = widgetRootData->dimSize;
                            element->SetUnifiedSize(widgetInstanceData->dimSize);
                        }
                        else
                        {
                            widgetInstanceData->dimSize = UDim2(Vector2::Zero_f, widgetRootData->size);
                            element->SetUnifiedSize(widgetInstanceData->dimSize);
                        }
                    }

                    RaiseDirty();
                }

                ImGui::BeginDisabled(!widgetInstanceData->overrideSize);
                if (ImGui::InputFloat4("Unified Size", &widgetInstanceData->dimSize))
                {
                    element->SetUnifiedSize(widgetInstanceData->dimSize);
                    RaiseDirty();
                }
                ImGui::EndDisabled();

                // Rotation
                if (checkOverrideSetting(overrideId, widgetInstanceData->overrideRotation))
                {
                    if (!widgetInstanceData->overrideRotation)
                    {
                        widgetInstanceData->rotation = 0.f;
                        element->SetRotation(widgetRootData->rotation);
                    }
                    else
                    {
                        widgetInstanceData->rotation = widgetRootData->rotation;
                        element->SetRotation(widgetInstanceData->rotation);
                    }

                    RaiseDirty();
                }

                ImGui::BeginDisabled(!widgetInstanceData->overrideRotation);
                if (ImGui::InputFloat("Rotation", &widgetInstanceData->rotation))
                {
                    element->SetRotation(widgetInstanceData->rotation);
                    RaiseDirty();
                }
                ImGui::EndDisabled();

                // Flip
                if (checkOverrideSetting(overrideId, widgetInstanceData->overrideFlipV))
                {
                    if (!widgetInstanceData->overrideFlipV)
                    {
                        widgetInstanceData->flipV = false;
                        element->SetFlipV(widgetRootData->flipV);
                    }
                    else
                    {
                        widgetInstanceData->flipV = widgetRootData->flipV;
                        element->SetFlipV(widgetInstanceData->flipV);
                    }

                    RaiseDirty();
                }

                ImGui::BeginDisabled(!widgetInstanceData->overrideFlipV);
                if (ImGui::Checkbox("Flip V", &widgetInstanceData->flipV))
                {
                    element->SetFlipV(widgetInstanceData->flipV);
                    RaiseDirty();
                }
                ImGui::EndDisabled();

                if (checkOverrideSetting(overrideId, widgetInstanceData->overrideFlipH))
                {
                    if (!widgetInstanceData->overrideFlipH)
                    {
                        widgetInstanceData->flipH = false;
                        element->SetFlipH(widgetRootData->flipH);
                    }
                    else
                    {
                        widgetInstanceData->flipH = widgetRootData->flipH;
                        element->SetFlipH(widgetInstanceData->flipH);
                    }

                    RaiseDirty();
                }

                ImGui::BeginDisabled(!widgetInstanceData->overrideFlipH);
                if (ImGui::Checkbox("Flip H", &widgetInstanceData->flipH))
                {
                    element->SetFlipH(widgetInstanceData->flipH);
                    RaiseDirty();
                }
                ImGui::EndDisabled();
            }
        }

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
                elementSprite->SetSubRect(!subImage ? sf::IntRect() : subImage->GetRect(), false);
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
            ElementSpriteGroupData* ownerGroupData = dynamic_cast<ElementSpriteGroupData*>(elementSpriteGroupItemData->parent);

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
            elementText->SetText(elementTextData->text);
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

    if (needRebuildHierarchy)
    {
        RebuildHierarchy();
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
        unifiedDimensionsGenerator(componentElements[0], elementSpriteGroupData->cacheSpriteGroupComponents[0]
            , UDim2::POSITION_TOP_LEFT
            , UDim2::SIZE_ZERO + elementSpriteGroupData->cacheSpriteGroupComponents[0]->size);

        // Top center.
        unifiedDimensionsGenerator(componentElements[1], elementSpriteGroupData->cacheSpriteGroupComponents[1]
            , UDim2::POSITION_TOP_LEFT + Vector2f(elementSpriteGroupData->cacheSpriteGroupComponents[0]->size.x, 0.f)
            , UDim2(1.f, 0.f) + Vector2f(-elementSpriteGroupData->cacheSpriteGroupComponents[0]->size.x - elementSpriteGroupData->cacheSpriteGroupComponents[2]->size.x, elementSpriteGroupData->cacheSpriteGroupComponents[1]->size.y));

        // Top right.
        unifiedDimensionsGenerator(componentElements[2], elementSpriteGroupData->cacheSpriteGroupComponents[2]
            , UDim2::POSITION_TOP_RIGHT + Vector2f(-elementSpriteGroupData->cacheSpriteGroupComponents[2]->size.x, 0.f)
            , UDim2::SIZE_ZERO + elementSpriteGroupData->cacheSpriteGroupComponents[2]->size);

        // Center left.
        unifiedDimensionsGenerator(componentElements[3], elementSpriteGroupData->cacheSpriteGroupComponents[3]
            , UDim2::POSITION_TOP_LEFT + Vector2f(0.f, elementSpriteGroupData->cacheSpriteGroupComponents[0]->size.y)
            , UDim2(0.f, 1.f) + Vector2f(elementSpriteGroupData->cacheSpriteGroupComponents[3]->size.x, -elementSpriteGroupData->cacheSpriteGroupComponents[0]->size.y - elementSpriteGroupData->cacheSpriteGroupComponents[6]->size.y));

        // Center.
        unifiedDimensionsGenerator(componentElements[4], elementSpriteGroupData->cacheSpriteGroupComponents[4]
            , UDim2::POSITION_TOP_LEFT + Vector2f(elementSpriteGroupData->cacheSpriteGroupComponents[3]->size.x, elementSpriteGroupData->cacheSpriteGroupComponents[1]->size.y)
            , UDim2::SIZE_FULL + Vector2f(-elementSpriteGroupData->cacheSpriteGroupComponents[3]->size.x - elementSpriteGroupData->cacheSpriteGroupComponents[5]->size.x, -elementSpriteGroupData->cacheSpriteGroupComponents[1]->size.y - elementSpriteGroupData->cacheSpriteGroupComponents[7]->size.y));

        // Center right.
        unifiedDimensionsGenerator(componentElements[5], elementSpriteGroupData->cacheSpriteGroupComponents[5]
            , UDim2::POSITION_TOP_RIGHT + Vector2f(-elementSpriteGroupData->cacheSpriteGroupComponents[5]->size.x, elementSpriteGroupData->cacheSpriteGroupComponents[2]->size.y)
            , UDim2(0.f, 1.f) + Vector2f(elementSpriteGroupData->cacheSpriteGroupComponents[5]->size.x, -elementSpriteGroupData->cacheSpriteGroupComponents[2]->size.y - elementSpriteGroupData->cacheSpriteGroupComponents[8]->size.y));

        // Bottom left.
        unifiedDimensionsGenerator(componentElements[6], elementSpriteGroupData->cacheSpriteGroupComponents[6]
            , UDim2::POSITION_BOTTOM_LEFT + Vector2f(0.f, -elementSpriteGroupData->cacheSpriteGroupComponents[6]->size.y)
            , UDim2::SIZE_ZERO + elementSpriteGroupData->cacheSpriteGroupComponents[6]->size);

        // Bottom center.
        unifiedDimensionsGenerator(componentElements[7], elementSpriteGroupData->cacheSpriteGroupComponents[7]
            , UDim2::POSITION_BOTTOM_LEFT + Vector2f(elementSpriteGroupData->cacheSpriteGroupComponents[6]->size.x, -elementSpriteGroupData->cacheSpriteGroupComponents[7]->size.y)
            , UDim2(1.f, 0.f) + Vector2f(-elementSpriteGroupData->cacheSpriteGroupComponents[6]->size.x - elementSpriteGroupData->cacheSpriteGroupComponents[8]->size.x, elementSpriteGroupData->cacheSpriteGroupComponents[7]->size.y));

        // Bottom right.
        unifiedDimensionsGenerator(componentElements[8], elementSpriteGroupData->cacheSpriteGroupComponents[8]
            , UDim2::POSITION_BOTTOM_RIGHT + -elementSpriteGroupData->cacheSpriteGroupComponents[8]->size
            , UDim2::SIZE_ZERO + elementSpriteGroupData->cacheSpriteGroupComponents[8]->size);

        // Segments tiling.
        elementSpriteGroupData->cacheSpriteGroupComponents[1]->repeatTexture = m_tileTopSegment;
        componentElements[1]->SetRepeatTexture(m_tileTopSegment);

        elementSpriteGroupData->cacheSpriteGroupComponents[3]->repeatTexture = m_tileLeftSegment;
        componentElements[3]->SetRepeatTexture(m_tileLeftSegment);

        elementSpriteGroupData->cacheSpriteGroupComponents[5]->repeatTexture = m_tileRightSegment;
        componentElements[5]->SetRepeatTexture(m_tileRightSegment);

        elementSpriteGroupData->cacheSpriteGroupComponents[7]->repeatTexture = m_tileBottomSegment;
        componentElements[7]->SetRepeatTexture(m_tileBottomSegment);

        // Finalize.
        RaiseDirty();
    }
}

}   //namespace gugu
