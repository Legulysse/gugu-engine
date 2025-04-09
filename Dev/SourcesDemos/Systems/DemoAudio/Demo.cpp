////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Audio/AudioMixerGroupInstance.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Element/UI/ElementSlider.h"
#include "Gugu/Element/2D/ElementText.h"
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

    // Root.
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);

    // Column 1.
    ElementButton* button = nullptr;
    ElementSlider* slider = nullptr;
    ElementText* text = nullptr;
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
    slider->SetOnValueChanged(std::bind(&Demo::OnSliderChanged, this, (int)ESlider::GlobalVolume, slider));
    slider->SetPosition(posX, posY);

    text = slider->AddChild<ElementText>();
    text->SetText("Global");
    text->SetUnifiedPosition(UDim2::POSITION_TOP_RIGHT + Vector2f(20, 0));

    posY += gapY;
    slider = m_root->AddChild<ElementSlider>();
    slider->LoadFromWidget("Slider_01.widget.xml");
    slider->SetValueLimits(0, 200);
    slider->SetValue(100);
    slider->SetOnValueChanged(std::bind(&Demo::OnSliderChanged, this, (int)ESlider::MasterVolume, slider));
    slider->SetPosition(posX, posY);

    text = slider->AddChild<ElementText>();
    text->SetText("Master");
    text->SetUnifiedPosition(UDim2::POSITION_TOP_RIGHT + Vector2f(20, 0));

    posY += gapY;
    slider = m_root->AddChild<ElementSlider>();
    slider->LoadFromWidget("Slider_01.widget.xml");
    slider->SetValueLimits(0, 200);
    slider->SetValue(100);
    slider->SetOnValueChanged(std::bind(&Demo::OnSliderChanged, this, (int)ESlider::MusicVolume, slider));
    slider->SetPosition(posX, posY);

    text = slider->AddChild<ElementText>();
    text->SetText("Music");
    text->SetUnifiedPosition(UDim2::POSITION_TOP_RIGHT + Vector2f(20, 0));

    posY += gapY;
    slider = m_root->AddChild<ElementSlider>();
    slider->LoadFromWidget("Slider_01.widget.xml");
    slider->SetValueLimits(0, 200);
    slider->SetValue(100);
    slider->SetOnValueChanged(std::bind(&Demo::OnSliderChanged, this, (int)ESlider::UIVolume, slider));
    slider->SetPosition(posX, posY);

    text = slider->AddChild<ElementText>();
    text->SetText("UI");
    text->SetUnifiedPosition(UDim2::POSITION_TOP_RIGHT + Vector2f(20, 0));

    posY += gapY;
    slider = m_root->AddChild<ElementSlider>();
    slider->LoadFromWidget("Slider_01.widget.xml");
    slider->SetValueLimits(0, 200);
    slider->SetValue(100);
    slider->SetOnValueChanged(std::bind(&Demo::OnSliderChanged, this, (int)ESlider::GameplayVolume, slider));
    slider->SetPosition(posX, posY);

    text = slider->AddChild<ElementText>();
    text->SetText("Gameplay");
    text->SetUnifiedPosition(UDim2::POSITION_TOP_RIGHT + Vector2f(20, 0));
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
        parameters.audioClipId = "Bulle_01.ogg";
        parameters.mixerGroupId = "Gameplay.audiomixergroup.xml";
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
        parameters.audioClipId = "Legulysse_Colors.ogg";
        parameters.mixerGroupId = "Music.audiomixergroup.xml";
        parameters.volume = 0.5f;
        parameters.fadeOut = 1.f;
        parameters.fadeIn = 2.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::TrackAbyss)
    {
        MusicParameters parameters;
        parameters.audioClipId = "Legulysse_Abyss.ogg";
        parameters.mixerGroupId = "Music.audiomixergroup.xml";
        parameters.volume = 0.5f;
        parameters.fadeOut = 2.f;
        parameters.fadeIn = 4.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::TrackMighty3310Fight)
    {
        MusicParameters parameters;
        parameters.audioClipId = "Legulysse_Mighty3310_Fight.ogg";
        parameters.mixerGroupId = "Music.audiomixergroup.xml";
        parameters.fadeOut = 0.f;
        parameters.fadeIn = 0.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::TrackMighty3310Boss)
    {
        MusicParameters parameters;
        parameters.audioClipId = "Legulysse_Mighty3310_Boss.ogg";
        parameters.mixerGroupId = "Music.audiomixergroup.xml";
        parameters.fadeOut = 0.f;
        parameters.fadeIn = 0.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::TrackMighty3310Final)
    {
        MusicParameters parameters;
        parameters.audioClipId = "Legulysse_Mighty3310_Final.ogg";
        parameters.mixerGroupId = "Music.audiomixergroup.xml";
        parameters.fadeOut = 0.f;
        parameters.fadeIn = 0.f;
        parameters.loop = true;
        GetAudio()->PlayMusic(parameters);
    }
    else if (buttonId == EButton::PlaylistMighty3310)
    {
        MusicParameters parametersA;
        parametersA.audioClipId = "Legulysse_Mighty3310_Fight.ogg";
        parametersA.mixerGroupId = "Music.audiomixergroup.xml";
        parametersA.fadeOut = 0.f;
        parametersA.fadeIn = 0.f;

        MusicParameters parametersB;
        parametersB.audioClipId = "Legulysse_Mighty3310_Boss.ogg";
        parametersB.mixerGroupId = "Music.audiomixergroup.xml";
        parametersB.fadeOut = 0.f;
        parametersB.fadeIn = 0.f;

        MusicParameters parametersC;
        parametersC.audioClipId = "Legulysse_Mighty3310_Final.ogg";
        parametersC.mixerGroupId = "Music.audiomixergroup.xml";
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
    if (sliderId == ESlider::GlobalVolume)
    {
        GetAudio()->SetMasterVolume(slider->GetValue() * 0.01f);
    }
    else if (sliderId == ESlider::MasterVolume)
    {
        GetAudio()->GetMixerGroupInstance("Master.audiomixergroup.xml")->SetVolume(slider->GetValue() * 0.01f);
    }
    else if (sliderId == ESlider::MusicVolume)
    {
        GetAudio()->GetMixerGroupInstance("Music.audiomixergroup.xml")->SetVolume(slider->GetValue() * 0.01f);
    }
    else if (sliderId == ESlider::UIVolume)
    {
        GetAudio()->GetMixerGroupInstance("UI.audiomixergroup.xml")->SetVolume(slider->GetValue() * 0.01f);
    }
    else if (sliderId == ESlider::GameplayVolume)
    {
        GetAudio()->GetMixerGroupInstance("Gameplay.audiomixergroup.xml")->SetVolume(slider->GetValue() * 0.01f);
    }
}

}   //namespace demoproject
