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

    if (ImGui::Button("Import"))
    {
        // Constants
        std::string imageMagickPath = "../ImageMagick/magick.exe";

        unsigned int maxTextureUnitSize = sf::Texture::getMaximumSize();
        Vector2u maxTextureSize = Vector2u(maxTextureUnitSize, maxTextureUnitSize);

        std::string resizeFilter = "RobidouxSharp";

        std::string targetResizeDirectory = CombinePaths(m_targetDirectory, resizeFilter);
        std::string targetPackingDirectory = targetResizeDirectory;
        FileInfo targetPackingFile = FileInfo::FromString_utf8(CombinePaths(targetPackingDirectory, m_targetTextureName));

        // Clear target directory.
        RemoveFile(targetPackingFile.GetFileSystemPath());

        // Parse resize source directory.
        std::vector<std::string> parsedDirectories;
        GetDirectories(m_sourceDirectory, parsedDirectories, false);

        for (const auto& parsedDirectory : parsedDirectories)
        {
            std::string arguments = StringFormat("\"{0}/*.png\" -filter {2} -resize 20.5% -unsharp 0x1 -set filename:base \"%[basename]\" \"{1}/%[filename:base].png\""
                , parsedDirectory
                , targetResizeDirectory
                , resizeFilter
            );

            EnsureDirectoryExists(targetResizeDirectory);
            ExecuteCommand(imageMagickPath, arguments);
        }

        // Parse packing source files.
        std::vector<FileInfo> sourceFiles;
        GetFiles(targetResizeDirectory, sourceFiles, false);

        std::vector<sf::Image*> sourceImages;
        sf::Image* targetImage = nullptr;
        Vector2u maxImageSize = Vector2::Zero_u;

        for (const auto& sourceFile : sourceFiles)
        {
            sf::Image* sourceImage = new sf::Image;
            sourceImage->loadFromFile(sourceFile.GetFileSystemPath());
            sourceImages.push_back(sourceImage);

            maxImageSize.x = Max(maxImageSize.x, sourceImage->getSize().x);
            maxImageSize.y = Max(maxImageSize.y, sourceImage->getSize().y);
        }

        if (!sourceImages.empty())
        {
            EnsureDirectoryExists(targetPackingFile.GetDirectoryPath_utf8());

            // Compute target size (align to the nearest power of 2).
            Vector2u minimumImageSize = Vector2u(maxImageSize.x * (unsigned int)sourceImages.size(), maxImageSize.y);
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
            for (size_t i = 0; i < sourceImages.size(); ++i)
            {
                targetImage->copy(*sourceImages[i], (unsigned int)i * maxImageSize.x, 0);
            }

            // Save to file.
            targetImage->saveToFile(targetPackingFile.GetFileSystemPath());
        }

        // Release data.
        ClearStdVector(sourceImages);
        SafeDelete(targetImage);
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
