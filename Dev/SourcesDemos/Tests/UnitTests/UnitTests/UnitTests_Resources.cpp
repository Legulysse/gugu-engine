////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/System/Platform.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

void RunUnitTests_Resources(UnitTestResults* results)
{
    GUGU_UTEST_INIT("Resources", "UnitTests_Resources.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("Manager");
    {
        GUGU_UTEST_SILENT_CHECK(DirectoryExists("Assets/TestResources"));

        GUGU_UTEST_SUBSECTION("Existing Resources");
        {
            GUGU_UTEST_CHECK_TRUE(FileExists("Assets/TestResources/TextureResource.png"));
            GUGU_UTEST_CHECK_TRUE(GetResources()->HasResource("TextureResource.png"));
            GUGU_UTEST_CHECK_FALSE(GetResources()->IsResourceLoaded("TextureResource.png"));
            GUGU_UTEST_CHECK_NOT_NULL(GetResources()->GetTexture("TextureResource.png"));
            GUGU_UTEST_CHECK_TRUE(GetResources()->IsResourceLoaded("TextureResource.png"));
        }

        GUGU_UTEST_SUBSECTION("New Resources");
        {
            std::string tempImageSetResourceId = "TempImageSetResource.imageset.xml";
            std::string tempImageSetResourcePath = "Assets/TestResources/TempImageSetResource.imageset.xml";
            FileInfo tempImageSetResourceFileInfo = FileInfo::FromString_utf8(tempImageSetResourcePath);

            GUGU_UTEST_CHECK_FALSE(FileExists(tempImageSetResourcePath));
            GUGU_UTEST_CHECK_FALSE(GetResources()->HasResource(tempImageSetResourceId));
            GUGU_UTEST_CHECK_FALSE(GetResources()->IsResourceLoaded(tempImageSetResourceId));
            //GUGU_UTEST_CHECK_NULL(GetResources()->GetImageSet(tempImageSetResourceId));     // TODO: mechanism to check+absorb expected error logs.
            GUGU_UTEST_CHECK_FALSE(GetResources()->IsResourceLoaded(tempImageSetResourceId));

            ImageSet* tempImageSetResource = new ImageSet;
            GUGU_UTEST_CHECK_TRUE(GetResources()->AddResource(tempImageSetResource, tempImageSetResourceFileInfo));

            // Added resource is registered and already loaded.
            GUGU_UTEST_CHECK_TRUE(GetResources()->HasResource(tempImageSetResourceId));
            GUGU_UTEST_CHECK_TRUE(GetResources()->IsResourceLoaded(tempImageSetResourceId));     // Resources added during runtime should be already loaded.
            GUGU_UTEST_CHECK_NOT_NULL(GetResources()->GetImageSet(tempImageSetResourceId));
            GUGU_UTEST_CHECK_TRUE(GetResources()->IsResourceLoaded(tempImageSetResourceId));

            // Actual file only exists after a Save.
            GUGU_UTEST_CHECK_FALSE(FileExists(tempImageSetResourcePath));
            GUGU_UTEST_CHECK_TRUE(tempImageSetResource->SaveToFile());
            GUGU_UTEST_CHECK_TRUE(FileExists(tempImageSetResourcePath));

            // Remove then re-add the resource.
            GUGU_UTEST_CHECK_TRUE(GetResources()->RemoveResource(tempImageSetResourceId, false));
            GUGU_UTEST_CHECK_FALSE(GetResources()->HasResource(tempImageSetResourceId));
            GUGU_UTEST_CHECK_FALSE(GetResources()->IsResourceLoaded(tempImageSetResourceId));
            GUGU_UTEST_CHECK_TRUE(FileExists(tempImageSetResourcePath));
            GUGU_UTEST_CHECK_TRUE(GetResources()->AddResource(tempImageSetResource, tempImageSetResourceFileInfo));
            GUGU_UTEST_CHECK_TRUE(GetResources()->HasResource(tempImageSetResourceId));
            GUGU_UTEST_CHECK_TRUE(GetResources()->IsResourceLoaded(tempImageSetResourceId));
            GUGU_UTEST_CHECK_TRUE(FileExists(tempImageSetResourcePath));

            // Delete will remove and delete the resource and its file.
            GUGU_UTEST_CHECK_TRUE(GetResources()->DeleteResource(tempImageSetResourceId));
            tempImageSetResource = nullptr;

            GUGU_UTEST_CHECK_FALSE(FileExists(tempImageSetResourcePath));
            GUGU_UTEST_CHECK_FALSE(GetResources()->HasResource(tempImageSetResourceId));
            GUGU_UTEST_CHECK_FALSE(GetResources()->IsResourceLoaded(tempImageSetResourceId));
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
