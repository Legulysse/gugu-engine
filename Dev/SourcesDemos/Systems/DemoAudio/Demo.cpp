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
    ElementButton* button;
    ElementSlider* slider;
    float posX = 20.f;
    float posY = 20.f;
    float gapY = 48.f;

    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Bubbles 01");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::SoundBubbles01));
    button->SetPosition(posX, posY);

    posY += gapY;
    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Bubbles (Five)");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::SoundCueBubblesFive));
    button->SetPosition(posX, posY);

    posY += gapY;
    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Bubbles (Pitch)");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::SoundCueBubblesPitch));
    button->SetPosition(posX, posY);

    posY += 100.f;

    posY += gapY;
    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Quit");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::Exit));
    button->SetPosition(posX, posY);

    // Column 2.
    posX = 320.f;
    posY = 20.f;

    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Track Colors");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackColors));
    button->SetPosition(posX, posY);

    posY += gapY;
    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Track Abyss");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackAbyss));
    button->SetPosition(posX, posY);

    posY += 20.f;

    posY += gapY;
    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Track Mighty 3310 Fight");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackMighty3310Fight));
    button->SetPosition(posX, posY);

    posY += gapY;
    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Track Mighty 3310 Boss");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackMighty3310Boss));
    button->SetPosition(posX, posY);

    posY += gapY;
    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Track Mighty 3310 Final");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::TrackMighty3310Final));
    button->SetPosition(posX, posY);

    posY += 20.f;

    posY += gapY;
    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Playlist Mighty 3310");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::PlaylistMighty3310));
    button->SetPosition(posX, posY);

    posY += 50.f;

    posY += gapY;
    button = m_root->AddChild<ElementButton>();
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Stop Music");
    button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, (int)EButton::StopAll));
    button->SetPosition(posX, posY);

    // Column 3.
    posX = 640.f;
    posY = 20.f;

    slider = m_root->AddChild<ElementSlider>();
    slider->LoadFromWidget("Slider_01.widget.xml");
    slider->SetValueLimits(0, 200);
    slider->SetValue(100);
    slider->SetOnValueChanged(std::bind(&Demo::OnSliderChanged, this, (int)ESlider::MasterVolume, slider));
    slider->SetPosition(posX, posY);
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::OnButtonClick(int buttonId)
{
    if (buttonId == EButton::SoundBubbles01)
    {
        SoundParameters parameters;
        parameters.soundID = "Bulle_01.ogg";
        parameters.mixerGroupID = "Gameplay.audiomixergroup.xml";
        parameters.volume = 0.5f;
        GetAudio()->PlaySound(parameters);
    }
    else if (buttonId == EButton::SoundCueBubblesFive)
    {
        GetAudio()->PlaySoundCue("Bubbles_Five.soundcue.xml");
    }
    else if (buttonId == EButton::SoundCueBubblesPitch)
    {
        GetAudio()->PlaySoundCue("Bubbles_Pitch.soundcue.xml");
    }
    else if (buttonId == EButton::TrackColors)
    {
        MusicParameters parameters;
        parameters.musicID = "Legulysse_Colors.ogg";
        parameters.mixerGroupID = "Music.audiomixergroup.xml";
        parameters.volume = 0.5f;
        parameters.fadeOut = 1.f;
        parameters.fadeIn = 2.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::TrackAbyss)
    {
        MusicParameters parameters;
        parameters.musicID = "Legulysse_Abyss.ogg";
        parameters.mixerGroupID = "Music.audiomixergroup.xml";
        parameters.volume = 0.5f;
        parameters.fadeOut = 2.f;
        parameters.fadeIn = 4.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::TrackMighty3310Fight)
    {
        MusicParameters parameters;
        parameters.musicID = "Legulysse_Mighty3310_Fight.ogg";
        parameters.mixerGroupID = "Music.audiomixergroup.xml";
        parameters.fadeOut = 0.f;
        parameters.fadeIn = 0.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::TrackMighty3310Boss)
    {
        MusicParameters parameters;
        parameters.musicID = "Legulysse_Mighty3310_Boss.ogg";
        parameters.mixerGroupID = "Music.audiomixergroup.xml";
        parameters.fadeOut = 0.f;
        parameters.fadeIn = 0.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::TrackMighty3310Final)
    {
        MusicParameters parameters;
        parameters.musicID = "Legulysse_Mighty3310_Final.ogg";
        parameters.mixerGroupID = "Music.audiomixergroup.xml";
        parameters.fadeOut = 0.f;
        parameters.fadeIn = 0.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::PlaylistMighty3310)
    {
        MusicParameters parametersA;
        parametersA.musicID = "Legulysse_Mighty3310_Fight.ogg";
        parametersA.mixerGroupID = "Music.audiomixergroup.xml";
        parametersA.fadeOut = 0.f;
        parametersA.fadeIn = 0.f;

        MusicParameters parametersB;
        parametersB.musicID = "Legulysse_Mighty3310_Boss.ogg";
        parametersB.mixerGroupID = "Music.audiomixergroup.xml";
        parametersB.fadeOut = 0.f;
        parametersB.fadeIn = 0.f;

        MusicParameters parametersC;
        parametersC.musicID = "Legulysse_Mighty3310_Final.ogg";
        parametersC.mixerGroupID = "Music.audiomixergroup.xml";
        parametersC.fadeOut = 0.f;
        parametersC.fadeIn = 0.f;

        std::vector<MusicParameters> playlist;
        playlist.push_back(parametersA);
        playlist.push_back(parametersB);
        playlist.push_back(parametersC);
        GetAudio()->PlayMusicList(playlist, true);
    }
    else if (buttonId == EButton::StopAll)
    {
        GetAudio()->StopMusic();
    }
    else if (buttonId == EButton::Exit)
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
