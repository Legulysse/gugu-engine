////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Element/UI/ElementSlider.h"
#include "Gugu/System/Memory.h"

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
    RegisterEventHandler(GetGameWindow());

    // Setup Audio.
    GetAudio()->SetAudioMixer(GetResources()->GetAudioMixerGroup("Master.audiomixergroup.xml"));

    // Root.
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);

    // Column 1.
    ElementButton* pButton;
    ElementSlider* slider;
    float fPosX = 20.f;
    float fPosY = 20.f;
    float fGapY = 48.f;
    float fSpacingY = 30.f;

    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Bubbles 01");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::SoundBubbles01));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Bubbles (Five)");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::SoundCueBubblesFive));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Bubbles (Pitch)");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::SoundCueBubblesPitch));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += 100.f;

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Quit");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::Exit));
    pButton->SetPosition(fPosX, fPosY);

    // Column 2.
    fPosX = 320.f;
    fPosY = 20.f;

    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Track Colors");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackColors));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Track Abyss");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackAbyss));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += 20.f;

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Track Mighty 3310 Fight");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackMighty3310Fight));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Track Mighty 3310 Boss");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackMighty3310Boss));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Track Mighty 3310 Final");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackMighty3310Final));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += 20.f;

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Playlist Mighty 3310");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::PlaylistMighty3310));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += 50.f;

    fPosY += fGapY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Stop Music");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::StopAll));
    pButton->SetPosition(fPosX, fPosY);

    // Column 3.
    fPosX = 640.f;
    fPosY = 20.f;

    slider = m_root->AddChild<ElementSlider>();
    slider->LoadFromWidget("Slider_01.widget.xml");
    slider->SetValueLimits(0, 200);
    slider->SetValue(100);
    slider->SetOnValueChanged(std::bind(&Demo::OnSliderChanged, this, (int)ESlider::MasterVolume, slider));
    slider->SetPosition(fPosX, fPosY);
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::OnButtonClick(int _eButton)
{
    if (_eButton == EButton::SoundBubbles01)
    {
        SoundParameters parameters;
        parameters.soundID = "Bulle_01.ogg";
        parameters.mixerGroupID = "Gameplay.audiomixergroup.xml";
        parameters.volume = 0.5f;
        GetAudio()->PlaySound(parameters);
    }
    else if (_eButton == EButton::SoundCueBubblesFive)
    {
        GetAudio()->PlaySoundCue("Bubbles_Five.soundcue.xml");
    }
    else if (_eButton == EButton::SoundCueBubblesPitch)
    {
        GetAudio()->PlaySoundCue("Bubbles_Pitch.soundcue.xml");
    }
    else if (_eButton == EButton::TrackColors)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Colors.ogg";
        kParameters.mixerGroupID = "Music.audiomixergroup.xml";
        kParameters.volume = 0.5f;
        kParameters.fadeOut = 1.f;
        kParameters.fadeIn = 2.f;
        kParameters.loop = true;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::TrackAbyss)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Abyss.ogg";
        kParameters.mixerGroupID = "Music.audiomixergroup.xml";
        kParameters.volume = 0.5f;
        kParameters.fadeOut = 2.f;
        kParameters.fadeIn = 4.f;
        kParameters.loop = true;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::TrackMighty3310Fight)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Mighty3310_Fight.ogg";
        kParameters.mixerGroupID = "Music.audiomixergroup.xml";
        kParameters.fadeOut = 0.f;
        kParameters.fadeIn = 0.f;
        kParameters.loop = true;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::TrackMighty3310Boss)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Mighty3310_Boss.ogg";
        kParameters.mixerGroupID = "Music.audiomixergroup.xml";
        kParameters.fadeOut = 0.f;
        kParameters.fadeIn = 0.f;
        kParameters.loop = true;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::TrackMighty3310Final)
    {
        MusicParameters kParameters;
        kParameters.musicID = "Legulysse_Mighty3310_Final.ogg";
        kParameters.mixerGroupID = "Music.audiomixergroup.xml";
        kParameters.fadeOut = 0.f;
        kParameters.fadeIn = 0.f;
        kParameters.loop = true;
        GetAudio()->PlayMusic(kParameters);
    }
    else if (_eButton == EButton::PlaylistMighty3310)
    {
        MusicParameters kParametersA;
        kParametersA.musicID = "Legulysse_Mighty3310_Fight.ogg";
        kParametersA.mixerGroupID = "Music.audiomixergroup.xml";
        kParametersA.fadeOut = 0.f;
        kParametersA.fadeIn = 0.f;

        MusicParameters kParametersB;
        kParametersB.musicID = "Legulysse_Mighty3310_Boss.ogg";
        kParametersB.mixerGroupID = "Music.audiomixergroup.xml";
        kParametersB.fadeOut = 0.f;
        kParametersB.fadeIn = 0.f;

        MusicParameters kParametersC;
        kParametersC.musicID = "Legulysse_Mighty3310_Final.ogg";
        kParametersC.mixerGroupID = "Music.audiomixergroup.xml";
        kParametersC.fadeOut = 0.f;
        kParametersC.fadeIn = 0.f;

        std::vector<MusicParameters> vecPlaylist;
        vecPlaylist.push_back(kParametersA);
        vecPlaylist.push_back(kParametersB);
        vecPlaylist.push_back(kParametersC);
        GetAudio()->PlayMusicList(vecPlaylist, true);
    }
    else if (_eButton == EButton::StopAll)
    {
        GetAudio()->StopMusic();
    }
    else if (_eButton == EButton::Exit)
    {
        GetEngine()->StopMainLoop();
    }
}

void Demo::OnSliderChanged(int sliderId, ElementSlider* slider)
{
    if (sliderId == ESlider::MasterVolume)
    {
        GetAudio()->SetMasterVolume100(slider->GetValue());
    }
}

}   //namespace demoproject
