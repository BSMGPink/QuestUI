#include "CustomTypes/Components/ViewControllers/ModSettingsButtonsViewController.hpp"
#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/TextAnchor.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "HMUI/Touchable.hpp"
#include "BeatSaberUI.hpp"

#include "customlogger.hpp"

DEFINE_TYPE(QuestUI, ModSettingsButtonsViewController);

UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(UnityEngine::Transform* parent){
    UnityEngine::UI::HorizontalLayoutGroup* layoutGroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(parent);
    layoutGroup->set_childForceExpandHeight(false);
    layoutGroup->set_childForceExpandWidth(false);
    layoutGroup->set_childControlHeight(false);
    layoutGroup->set_childControlWidth(false);
    layoutGroup->set_childAlignment(UnityEngine::TextAnchor::MiddleLeft);
    layoutGroup->set_spacing(3.0f);
    return layoutGroup;
}

void QuestUI::ModSettingsButtonsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    getLogger().info("ModSettingsButtonsViewController DidActivate %d %d", firstActivation, addedToHierarchy);
    if(firstActivation && addedToHierarchy) {
        get_gameObject()->AddComponent<HMUI::Touchable*>();
        UnityEngine::GameObject* scrollView = BeatSaberUI::CreateScrollView(get_transform());
        ExternalComponents* externalComponents = scrollView->GetComponent<ExternalComponents*>();
        UnityEngine::RectTransform* scrollTransform = externalComponents->Get<UnityEngine::RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(-28.0f, 0.0f));
        UnityEngine::UI::HorizontalLayoutGroup* layoutGroup = nullptr;
        auto& vec = QuestUI::ModSettingsInfos::get();
        auto itr = std::find_if(vec.begin(), vec.end(), [](auto& x) -> bool{ return (x.location & Register::MenuLocation::Settings); });
        if (itr != vec.end()) {
            int currentItems = 0;
            for (auto& info : vec) {
                if (info.location & Register::MenuLocation::Settings) {
                    if(currentItems % 3 == 0)
                        layoutGroup = CreateHorizontalLayoutGroup(scrollView->get_transform());
                    BeatSaberUI::CreateUIButton(layoutGroup->get_transform(), info.title, UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(36.0f, 8.0f), 
                        [this, &info] {
                            getLogger().info("OnModSettingsButtonClick %s", info.modInfo.id.c_str());
                            if(this->OnOpenModSettings)
                                this->OnOpenModSettings(info);
                        }
                    );
                    currentItems++;
                }
            }
        }
    } 
}

