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
#include "Gugu/Debug/Logger.h"

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
    // - We want to trim empty spacings between frames, but with a symmetrical rule (keep all frames centered).
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

        std::string targetResizeDirectory = m_targetDirectory;  // CombinePaths(m_targetDirectory, resizeFilter);
        std::string targetPackingDirectory = targetResizeDirectory;
        FileInfo targetPackingFile = FileInfo::FromString_utf8(CombinePaths(targetPackingDirectory, m_targetTextureName));

        // Prepare Image data.
        std::vector<sf::Image*> sourceImages;
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
            EnsureDirectoryExists(targetPackingFile.GetDirectoryPath_utf8());

            sf::Image* targetImage = nullptr;

            // Compute minimum width with trim for source images.
            // This is meant to handle frames with varying size.
            // All frames greater than this size can be trimmed.
            Vector2i maxFrameImageSize = Vector2::Zero_i;
            int maxTrimmedWidth = 0;

            for (const auto& sourceImage : sourceImages)
            {
                int width = sourceImage->getSize().x;
                int height = sourceImage->getSize().y;
                bool canTrimHorizontally = true;
                int horizontalTrim = 0;

                while (canTrimHorizontally && horizontalTrim * 2 < width)
                {
                    for (int y = 0; y < height; ++y)
                    {
                        canTrimHorizontally &= sourceImage->getPixel(horizontalTrim, y) == sf::Color::Transparent;
                        canTrimHorizontally &= sourceImage->getPixel(width - 1 - horizontalTrim, y) == sf::Color::Transparent;
                    }

                    if (canTrimHorizontally)
                    {
                        ++horizontalTrim;
                    }
                }

                int trimmedWidth = width - horizontalTrim * 2;
                maxTrimmedWidth = Max(maxTrimmedWidth, trimmedWidth);

                maxFrameImageSize.x = Max(maxFrameImageSize.x, maxTrimmedWidth);
                maxFrameImageSize.y = Max(maxFrameImageSize.y, height);
            }

            // Compute target size (align to the nearest power of 2).
            int maxFrameCount = 0;
            for (const auto& frameCount : frameCountPerAnimation)
            {
                maxFrameCount = Max(frameCount, maxFrameCount);
            }

            int width = maxFrameCount * maxFrameImageSize.x;
            int height = (int)frameCountPerAnimation.size() * maxFrameImageSize.y;
            Vector2u minTargetImageSize = Vector2u(width, height);
            Vector2u targetImageSize = Vector2u(2, 2);

            while (targetImageSize.x < maxTextureSize.x && targetImageSize.x < minTargetImageSize.x)
            {
                targetImageSize.x *= 2;
            }

            while (targetImageSize.y < maxTextureSize.y && targetImageSize.y < minTargetImageSize.y)
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
                // If offset < 0 then the source is larger than the target destination.
                // If offset > 0 then the source is smaller than the target destination.
                int frameOffsetX = 0;
                if ((int)sourceImages[i]->getSize().x != maxFrameImageSize.x)
                    frameOffsetX = RoundNearestInt(RoundCeil((maxFrameImageSize.x - (int)sourceImages[i]->getSize().x) * 0.5f));

                int frameOffsetY = 0;
                if ((int)sourceImages[i]->getSize().y != maxFrameImageSize.y)
                    frameOffsetY = RoundNearestInt(RoundCeil((maxFrameImageSize.y - (int)sourceImages[i]->getSize().y) * 0.5f));

                int frameDestOffsetX = Max<int>(0, frameOffsetX);
                int frameDestOffsetY = Max<int>(0, frameOffsetY);
                Vector2u frameRectOffset = Vector2u(Absolute(Min<int>(0, frameOffsetX)), Absolute(Min<int>(0, frameOffsetY)));
                Vector2u frameRectSize = sourceImages[i]->getSize() - Vector2u(frameRectOffset.x * 2, frameRectOffset.y * 2);
                sf::IntRect frameRect = sf::IntRect(Vector2i(frameRectOffset), Vector2i(frameRectSize));

                targetImage->copy(*sourceImages[i],
                    frameIndex * maxFrameImageSize.x + frameDestOffsetX,
                    animationIndex * maxFrameImageSize.y + frameDestOffsetY,
                    frameRect);
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

            // Generate Notes file.
            FileInfo resultNotesPath = FileInfo::FromString_utf8(StringFormat("{0}.txt", CombinePaths(targetPackingFile.GetDirectoryPath_utf8(), targetPackingFile.GetPrettyName())));
            
            RemoveFile(resultNotesPath.GetFileSystemPath());
            WriteInFileEndline(resultNotesPath.GetFileSystemPath(), StringFormat("Frame Size: {0} x {1}", maxFrameImageSize.x, maxFrameImageSize.y));
            WriteInFileEndline(resultNotesPath.GetFileSystemPath(), StringFormat("Scale: {0}%", m_resizeScale));
            WriteInFileEndline(resultNotesPath.GetFileSystemPath(), StringFormat("Filter: {0}", resizeFilter));

            GetLogEngine()->Print(ELog::Info, ELogEngine::Editor, StringFormat("Imported ImageSet Succesful, frame size = {0}", maxFrameImageSize));
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
