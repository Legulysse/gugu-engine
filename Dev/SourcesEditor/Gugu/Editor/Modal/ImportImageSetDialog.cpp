////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/ImportImageSetDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Container.h"
#include "Gugu/System/Path.h"
#include "Gugu/System/Platform.h"
#include "Gugu/System/String.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/External/ImGuiUtility.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ImportImageSetDialog::ImportImageSetDialog()
    : BaseModalDialog("Import ImageSet")
    , m_resizeScale(100.f)
    , m_filterIndex(0)
{
    // TODO: default directories
    m_sourceDirectory = "../ImportTest/warrior_source";
    m_targetDirectory = "../ImportTest/warrior_source_packed";
    m_targetTextureName = "ImageSetTexture.png";
}

ImportImageSetDialog::~ImportImageSetDialog()
{
}

void ImportImageSetDialog::UpdateModalImpl(const DeltaTime& dt)
{
    // Notes: This WIP implementation assumes those settings:
    // - Animation frames are stored in subdirectories of the source directory.
    // - No other texture will be present in the source directory.
    // - Target directory will be outside of the source directory.
    // - We want one row per animation in the final texture.
    // - We want all individual frames to have the same size.
    // - We want a final texture size aligned on a power of 2 (not necessarily squared).
    // - ImageMagick has been setup in a predefined directory.
    // - We always apply an unsharp pass after the resize.

    ImGui::PushItemWidth(800);
    ImGui::InputText("Source Directory", &m_sourceDirectory);
    ImGui::InputText("Target Directory", &m_targetDirectory);
    ImGui::InputText("Target Texture Name", &m_targetTextureName);
    ImGui::PopItemWidth();

    ImGui::InputFloat("Scale (%)", &m_resizeScale);

    enum EFilter
    {
        RobidouxSharp,
        Jinc,
        Triangle,
    };
    static const std::vector<std::string> filters = { "RobidouxSharp", "Jinc", "Triangle" };

    size_t dummyIndex = static_cast<size_t>(m_filterIndex);
    if (ImGui::Combo("Filter", filters, &dummyIndex))
    {
        m_filterIndex = static_cast<EFilter>(dummyIndex);
    }

    if (ImGui::Button("Import"))
    {
        // Constants
        std::string imageMagickPath = "../ImageMagick/magick.exe";

        unsigned int maxTextureUnitSize = sf::Texture::getMaximumSize();
        Vector2u maxTextureSize = Vector2u(maxTextureUnitSize, maxTextureUnitSize);

        std::string resizeFilter = filters[m_filterIndex];

        std::string targetResizeDirectory = CombinePaths(m_targetDirectory, resizeFilter);
        std::string targetPackingDirectory = targetResizeDirectory;
        FileInfo targetPackingFile = FileInfo::FromString_utf8(CombinePaths(targetPackingDirectory, m_targetTextureName));

        // Prepare Image data.
        std::vector<sf::Image*> sourceImages;
        sf::Image* targetImage = nullptr;
        std::vector<int> frameCountPerAnimation;

        // Clear target directory.
        RemoveFile(targetPackingFile.GetFileSystemPath());

        // Parse resize source directory.
        std::vector<std::string> parsedDirectories;
        GetDirectories(m_sourceDirectory, parsedDirectories, false);

        for (const auto& parsedDirectory : parsedDirectories)
        {
            std::vector<FileInfo> parsedFiles;
            GetFiles(parsedDirectory, parsedFiles, false);

            std::string targetAnimationDirectory = CombinePaths(targetResizeDirectory, NamePartFromPath(parsedDirectory));
            EnsureDirectoryExists(targetAnimationDirectory);

            int frameCount = 0;
            for (const auto& parsedFile : parsedFiles)
            {
                if (!parsedFile.HasExtension("png"))
                    continue;

                FileInfo targetFilePath = FileInfo::FromString_utf8(CombinePaths(targetAnimationDirectory, parsedFile.GetFileName_utf8()));

                std::string arguments = StringFormat("\"{0}\" -filter {2} -resize {3}% -unsharp 0x1 \"{1}\""
                    , parsedFile.GetFilePath_utf8()
                    , targetFilePath.GetFilePath_utf8()
                    , resizeFilter
                    , m_resizeScale
                );

                if (ExecuteCommand(imageMagickPath, arguments))
                {
                    sf::Image* sourceImage = new sf::Image;
                    sourceImage->loadFromFile(targetFilePath.GetFileSystemPath());
                    sourceImages.push_back(sourceImage);

                    ++frameCount;
                }
            }

            if (frameCount > 0)
            {
                frameCountPerAnimation.push_back(frameCount);
            }
        }

        // Parse packing source files.
        if (!sourceImages.empty())
        {
            Vector2u maxImageSize = Vector2::Zero_u;
            for (const auto& sourceImage : sourceImages)
            {
                maxImageSize.x = Max(maxImageSize.x, sourceImage->getSize().x);
                maxImageSize.y = Max(maxImageSize.y, sourceImage->getSize().y);
            }

            int maxFrameCount = 0;
            for (const auto& frameCount : frameCountPerAnimation)
            {
                maxFrameCount = Max(frameCount, maxFrameCount);
            }

            EnsureDirectoryExists(targetPackingFile.GetDirectoryPath_utf8());

            // Compute target size (align to the nearest power of 2).
            int width = maxFrameCount * maxImageSize.x;
            int height = (int)frameCountPerAnimation.size() * maxImageSize.y;
            Vector2u minimumImageSize = Vector2u(width, height);
            Vector2u targetImageSize = Vector2u(2, 2);

            while (targetImageSize.x < maxTextureSize.x && targetImageSize.x < minimumImageSize.x)
            {
                targetImageSize.x *= 2;
            }

            while (targetImageSize.y < maxTextureSize.y && targetImageSize.y < minimumImageSize.y)
            {
                targetImageSize.y *= 2;
            }

            // Prepare target image.
            targetImage = new sf::Image;
            targetImage->create(targetImageSize.x, targetImageSize.y, sf::Color::Transparent);

            // Fill target image with all subImages.
            int animationIndex = 0;
            int frameIndex = 0;
            for (size_t i = 0; i < sourceImages.size(); ++i)
            {
                targetImage->copy(*sourceImages[i], frameIndex * maxImageSize.x, animationIndex * maxImageSize.y);
                ++frameIndex;

                if (frameIndex >= frameCountPerAnimation[animationIndex])
                {
                    ++animationIndex;
                    frameIndex = 0;
                }
            }

            // Save to file.
            targetImage->saveToFile(targetPackingFile.GetFileSystemPath());

            // Release data.
            ClearStdVector(sourceImages);
            SafeDelete(targetImage);
        }
    }

    ImGui::Spacing();
    if (ImGui::Button("Cancel"))
    {
        CloseModalImpl();
    }

    ImGui::SameLine();
    if (ImGui::Button("Validate"))  // TODO: finalize buttons.
    {
        CloseModalImpl();
    }
}

}   //namespace gugu
