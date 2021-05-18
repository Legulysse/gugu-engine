////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Misc/Action.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Demo::Demo()
: m_root(nullptr)
{
}

Demo::~Demo()
{
}

void Demo::AppStart()
{
    RegisterHandlerEvents(GetGameWindow());

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    //Menu Left
    ElementButton* pButton;
    float fPosX = 20.f;
    float fPosY = 20.f;
    float fGapY = 64.f;

    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Bubbles");
    pButton->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, (int)EButton::SoundBubbles));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += 20.f;

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Track Colors");
    pButton->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, (int)EButton::TrackColors));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Track Abyss");
    pButton->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, (int)EButton::TrackAbyss));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Stop Music");
    pButton->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, (int)EButton::StopAll));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += 20.f;

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Quit");
    pButton->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, (int)EButton::Exit));
    pButton->SetPosition(fPosX, fPosY);

    //Second column

    fPosX = 320.f;
    fPosY = 20.f;

    fPosY += 20.f;

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Track Mighty 3310 Fight");
    pButton->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, (int)EButton::TrackMighty3310Fight));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Track Mighty 3310 Boss");
    pButton->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, (int)EButton::TrackMighty3310Boss));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Track Mighty 3310 Final");
    pButton->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, (int)EButton::TrackMighty3310Final));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Playlist Mighty 3310");
    pButton->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, (int)EButton::PlaylistMighty3310));
    pButton->SetPosition(fPosX, fPosY);
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::OnButtonClick(int _eButton)
{
    //EButton eButton = (EButton)_eButton;

    if (_eButton == EButton::SoundBubbles)
    {
        GetAudio()->PlaySoundCue("Bubbles.sound");
    }
    else if (_eButton == EButton::TrackColors)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Colors.ogg";
        kParameters.volume = 0.5f;
        kParameters.fadeOut = 1.f;
        kParameters.fadeIn = 2.f;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::TrackAbyss)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Abyss.ogg";
        kParameters.volume = 0.5f;
        kParameters.fadeOut = 2.f;
        kParameters.fadeIn = 4.f;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::TrackMighty3310Fight)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Mighty3310_Fight.ogg";
        kParameters.fadeOut = 0.f;
        kParameters.fadeIn = 0.f;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::TrackMighty3310Boss)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Mighty3310_Boss.ogg";
        kParameters.fadeOut = 0.f;
        kParameters.fadeIn = 0.f;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::TrackMighty3310Final)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Mighty3310_Final.ogg";
        kParameters.fadeOut = 0.f;
        kParameters.fadeIn = 0.f;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::PlaylistMighty3310)
    {
        MusicParameters kParametersA;
        kParametersA.musicID = "Legulysse_Mighty3310_Fight.ogg";
        kParametersA.fadeOut = 0.f;
        kParametersA.fadeIn = 0.f;

        MusicParameters kParametersB;
        kParametersB.musicID = "Legulysse_Mighty3310_Boss.ogg";
        kParametersB.fadeOut = 0.f;
        kParametersB.fadeIn = 0.f;

        MusicParameters kParametersC;
        kParametersC.musicID = "Legulysse_Mighty3310_Final.ogg";
        kParametersC.fadeOut = 0.f;
        kParametersC.fadeIn = 0.f;

        std::vector<MusicParameters> vecPlaylist;
        vecPlaylist.push_back(kParametersA);
        vecPlaylist.push_back(kParametersB);
        vecPlaylist.push_back(kParametersC);
        GetAudio()->PlayMusicList(vecPlaylist);
    }
    else if (_eButton == EButton::StopAll)
    {
        GetAudio()->StopMusic();
    }
    else if (_eButton == EButton::Exit)
    {
        GetEngine()->StopLooping();
    }
}

}   //namespace demoproject
