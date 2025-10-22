////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/ImportImageSetDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"

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

namespace impl {

enum EResizeFilter
{
    RobidouxSharp,
    Jinc,
    Triangle,
};

static const std::vector<std::string> ResizeFilters = { "RobidouxSharp", "Jinc", "Triangle" };

}   // namespace impl

ImportImageSetDialog::ImportImageSetDialog()
    : BaseModalDialog("Import ImageSet")
    , m_resizeScale(100.f)
    , m_resizeFilterIndex(0)
{
    // Default settings.
    m_sourceDirectory = "../PlaceholderSourceDirectory";
    m_targetDirectory = "../PlaceholderTargetDirectory";
    m_targetTextureName = "ImageSetTexture.png";

    // User settings.
    if (!GetEditor()->GetUserSettings().importImageSetSourceDirectoryPath.empty())
    {
        m_sourceDirectory = GetEditor()->GetUserSettings().importImageSetSourceDirectoryPath;
    }

    if (!GetEditor()->GetUserSettings().importImageSetTargetDirectoryPath.empty())
    {
        m_targetDirectory = GetEditor()->GetUserSettings().importImageSetTargetDirectoryPath;
    }

    if (!GetEditor()->GetUserSettings().importImageSetTargetTextureName.empty())
    {
        m_targetTextureName = GetEditor()->GetUserSettings().importImageSetTargetTextureName;
    }

    if (ApproxSuperiorToZero(GetEditor()->GetUserSettings().importImageSetResizeScale, math::Epsilon3))
    {
        m_resizeScale = GetEditor()->GetUserSettings().importImageSetResizeScale;
    }

    if (!GetEditor()->GetUserSettings().importImageSetResizeFilter.empty())
    {
        m_resizeFilterIndex = StdVectorIndexOf(impl::ResizeFilters, GetEditor()->GetUserSettings().importImageSetResizeFilter);
        m_resizeFilterIndex = m_resizeFilterIndex == system::InvalidIndex ? 0 : m_resizeFilterIndex;
    }
}

ImportImageSetDialog::~ImportImageSetDialog()
{
}

void ImportImageSetDialog::UpdateModalImpl(const DeltaTime& dt)
{
    ImGui::PushItemWidth(800);
    ImGui::InputText("Source Directory", &m_sourceDirectory);
    ImGui::InputText("Target Directory", &m_targetDirectory);
    ImGui::InputText("Target Texture Name", &m_targetTextureName);
    ImGui::PopItemWidth();

    ImGui::InputFloat("Scale (%)", &m_resizeScale);


    size_t dummyIndex = static_cast<size_t>(m_resizeFilterIndex);
    if (ImGui::Combo("Filter", impl::ResizeFilters, &dummyIndex))
    {
        m_resizeFilterIndex = static_cast<impl::EResizeFilter>(dummyIndex);
    }

    ImGui::Spacing();
    if (ImGui::Button("Cancel"))
    {
        CloseModalImpl();
    }

    ImGui::SameLine();
    if (ImGui::Button("Import"))
    {
        ImportImageSet(impl::ResizeFilters[m_resizeFilterIndex]);
        CloseModalImpl();
    }
}

void ImportImageSetDialog::ImportImageSet(const std::string& resizeFilter)
{
    // Notes: This WIP implementation assumes those settings:
    // - Animation frames are stored in subdirectories of the source directory.
    // - No other texture will be present in the source directory.
    // - Target directory will be outside of the source directory.
    // - We want one row per animation in the final texture.
    // - We want all individual frames to have the same size, with dimensions as multiples of 2.
    // - We want all individual frames to have an outline of 1 transparent pixel.
    // - We want to trim empty spacings between frames, but with a symmetrical rule (keep all frames centered).
    // - We want a final texture size aligned on a power of 2 (not necessarily squared).
    // - ImageMagick has been setup in a predefined directory.
    // - We always apply an unsharp pass after the resize.
    //
    // TODO:
    // - Nothing prevents the user from replacing the original textures with the resized textures.
    // - Flip option.

    // Save settings.
    GetEditor()->GetUserSettings().importImageSetSourceDirectoryPath = m_sourceDirectory;
    GetEditor()->GetUserSettings().importImageSetTargetDirectoryPath = m_targetDirectory;
    GetEditor()->GetUserSettings().importImageSetTargetTextureName = m_targetTextureName;
    GetEditor()->GetUserSettings().importImageSetResizeScale = m_resizeScale;
    GetEditor()->GetUserSettings().importImageSetResizeFilter = resizeFilter;
    GetEditor()->SaveUserSettings();

    // Constants
#ifdef GUGU_OS_WINDOWS
    std::string imageMagickPath = CombinePaths(GetEditor()->GetEditorConfig().imageMagickDirectoryPath, "magick.exe");
#endif
    if (!FileExists(imageMagickPath))
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Editor, StringFormat("ImageMagick executable not found : {0}", imageMagickPath));
        return;
    }

    if (m_sourceDirectory == m_targetDirectory || m_sourceDirectory.empty() || m_targetDirectory.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Editor, StringFormat("Source and target directories must be different and not empty : source=\"{0}\" target=\"{1}\"", m_sourceDirectory, m_targetDirectory));
        return;
    }

    unsigned int maxTextureUnitSize = sf::Texture::getMaximumSize();
    Vector2u maxTextureSize = Vector2u(maxTextureUnitSize, maxTextureUnitSize);

    std::string targetResizeDirectory = m_targetDirectory;  // CombinePaths(m_targetDirectory, resizeFilter);
    std::string targetPackingDirectory = targetResizeDirectory;
    FileInfo targetPackingFile = FileInfo::FromString_utf8(CombinePaths(targetPackingDirectory, m_targetTextureName));

    // Prepare Image data.
    std::vector<sf::Image*> frameImages;
    std::vector<int> frameCountPerAnimation;

    // Clear target directory.
    RemoveFile(targetPackingFile.GetFileSystemPath());

    // Parse resize source directory.
    std::vector<std::string> parsedDirectories;
    GetDirectories(m_sourceDirectory, parsedDirectories, false);

    if (parsedDirectories.empty())
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Editor, "The source directory has no animation subdirectories, fallback on source directory itself");
        
        parsedDirectories.push_back(m_sourceDirectory);
    }

    for (const auto& parsedDirectory : parsedDirectories)
    {
        std::vector<FileInfo> parsedFiles;
        GetFiles(parsedDirectory, parsedFiles, false);

        std::string targetResizedAnimationDirectory = CombinePaths(targetResizeDirectory, NamePartFromPath(parsedDirectory));
        EnsureDirectoryExists(targetResizedAnimationDirectory);

        int frameCount = 0;
        for (const auto& parsedFile : parsedFiles)
        {
            if (!parsedFile.HasExtension("png"))
                continue;

            FileInfo targetResizedFile = FileInfo::FromString_utf8(CombinePaths(targetResizedAnimationDirectory, parsedFile.GetFileName_utf8()));

            std::string arguments = StringFormat("\"{0}\" -filter {2} -resize {3}% -unsharp 0x1 \"{1}\""
                , parsedFile.GetFilePath_utf8()
                , targetResizedFile.GetFilePath_utf8()
                , resizeFilter
                , m_resizeScale
            );

            if (ExecuteCommand(imageMagickPath, arguments))
            {
                // TODO: skip image if failed ? add placeholder ?
                sf::Image* frameImage = new sf::Image;
                bool loadFrameImageResult = frameImage->loadFromFile(targetResizedFile.GetFileSystemPath());
                frameImages.push_back(frameImage);

                ++frameCount;
            }
        }

        if (frameCount > 0)
        {
            frameCountPerAnimation.push_back(frameCount);
        }
    }

    // Parse packing source files.
    if (frameImages.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Editor, "No images found in the source directories");
    }
    else
    {
        EnsureDirectoryExists(targetPackingFile.GetDirectoryPath_utf8());

        sf::Image* targetImage = nullptr;

        // Compute minimum width with trim for source images.
        // This is meant to handle frames with varying size.
        // All frames greater than this size can be trimmed.
        Vector2i maxFrameSize = Vector2::Zero_i;

        for (const auto& sourceImage : frameImages)
        {
            int width = sourceImage->getSize().x;
            int height = sourceImage->getSize().y;
            bool canTrimHorizontally = true;
            bool canTrimVertically = true;
            int horizontalTrim = 0;
            int verticalTrim = 0;

            while (canTrimHorizontally && horizontalTrim * 2 < width)
            {
                for (int y = 0; y < height; ++y)
                {
                    canTrimHorizontally &= sourceImage->getPixel(Vector2u(horizontalTrim, y)) == sf::Color::Transparent;
                    canTrimHorizontally &= sourceImage->getPixel(Vector2u(width - 1 - horizontalTrim, y)) == sf::Color::Transparent;
                }

                if (canTrimHorizontally)
                {
                    ++horizontalTrim;
                }
            }

            while (canTrimVertically && verticalTrim * 2 < height)
            {
                for (int x = 0; x < width; ++x)
                {
                    canTrimVertically &= sourceImage->getPixel(Vector2u(x, verticalTrim)) == sf::Color::Transparent;
                    canTrimVertically &= sourceImage->getPixel(Vector2u(x, height - 1 - verticalTrim)) == sf::Color::Transparent;
                }

                if (canTrimVertically)
                {
                    ++verticalTrim;
                }
            }

            // Trimmed size is the minimum size we could use after a trim.
            // Ideal size will ensure we add 1 transparent pixel around each frame.
            int trimmedWidth = width - horizontalTrim * 2;
            int trimmedHeight = height - verticalTrim * 2;
            int idealWidth = trimmedWidth + 2;
            int idealHeight = trimmedHeight + 2;

            maxFrameSize.x = Max(maxFrameSize.x, idealWidth);
            maxFrameSize.y = Max(maxFrameSize.y, idealHeight);
        }

        // Force a multiple of 2.
        maxFrameSize.x = maxFrameSize.x % 2 == 1 ? maxFrameSize.x + 1 : maxFrameSize.x;
        maxFrameSize.y = maxFrameSize.y % 2 == 1 ? maxFrameSize.y + 1 : maxFrameSize.y;

        // Compute target size (align to the nearest power of 2).
        int maxFrameCount = 0;
        for (const auto& frameCount : frameCountPerAnimation)
        {
            maxFrameCount = Max(frameCount, maxFrameCount);
        }

        int width = maxFrameCount * maxFrameSize.x;
        int height = (int)frameCountPerAnimation.size() * maxFrameSize.y;
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
        targetImage = new sf::Image(targetImageSize, sf::Color::Transparent);

        // Fill target image with all subImages.
        int animationIndex = 0;
        int frameIndex = 0;
        for (size_t i = 0; i < frameImages.size(); ++i)
        {
            // If offset < 0 then the source is larger than the target destination.
            // If offset > 0 then the source is smaller than the target destination.
            int frameOffsetX = 0;
            if ((int)frameImages[i]->getSize().x != maxFrameSize.x)
                frameOffsetX = RoundNearestInt(RoundFloor((maxFrameSize.x - (int)frameImages[i]->getSize().x) * 0.5f));

            int frameOffsetY = 0;
            if ((int)frameImages[i]->getSize().y != maxFrameSize.y)
                frameOffsetY = RoundNearestInt(RoundFloor((maxFrameSize.y - (int)frameImages[i]->getSize().y) * 0.5f));

            int frameDestOffsetX = Max<int>(0, frameOffsetX);
            int frameDestOffsetY = Max<int>(0, frameOffsetY);
            Vector2u frameRectOffset = Vector2u(Absolute(Min<int>(0, frameOffsetX)), Absolute(Min<int>(0, frameOffsetY)));
            Vector2u frameRectSize = frameImages[i]->getSize() - Vector2u(frameRectOffset.x * 2, frameRectOffset.y * 2);
            sf::IntRect frameRect = sf::IntRect(Vector2i(frameRectOffset), Vector2i(frameRectSize));
            Vector2u framePosition = Vector2u(frameIndex * maxFrameSize.x + frameDestOffsetX, animationIndex * maxFrameSize.y + frameDestOffsetY);

            // TODO: skip image if failed ? add placeholder ?
            bool copyTargetImageResult = targetImage->copy(*frameImages[i],
                framePosition,
                frameRect);

            ++frameIndex;
            if (frameIndex >= frameCountPerAnimation[animationIndex])
            {
                ++animationIndex;
                frameIndex = 0;
            }
        }

        // Save to file.
        bool saveTargetImageResult = targetImage->saveToFile(targetPackingFile.GetFileSystemPath());

        // Release data.
        ClearStdVector(frameImages);
        SafeDelete(targetImage);

        // Generate Notes file.
        FileInfo resultNotesPath = FileInfo::FromString_utf8(StringFormat("{0}.txt", CombinePaths(targetPackingFile.GetDirectoryPath_utf8(), targetPackingFile.GetPrettyName())));

        RemoveFile(resultNotesPath.GetFileSystemPath());
        WriteInFileEndline(resultNotesPath.GetFileSystemPath(), StringFormat("Frame Size: {0} x {1}", maxFrameSize.x, maxFrameSize.y));
        WriteInFileEndline(resultNotesPath.GetFileSystemPath(), StringFormat("Scale: {0}%", m_resizeScale));
        WriteInFileEndline(resultNotesPath.GetFileSystemPath(), StringFormat("Filter: {0}", resizeFilter));

        GetLogEngine()->Print(ELog::Info, ELogEngine::Editor, StringFormat("Imported ImageSet Succesful, frame size = {0}", maxFrameSize));
    }
}

}   // namespace gugu
