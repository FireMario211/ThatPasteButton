#include <Geode/Geode.hpp>
#include <Geode/modify/SetGroupIDLayer.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {

	struct Fields {
		SEL_MenuHandler m_pasteStateCallback;
		CCObject* m_pasteStateTarget;

		SEL_MenuHandler m_pasteColorCallback;
		CCObject* m_pasteColorTarget;
	};

    bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;

		if (Mod::get()->getSettingValue<bool>("toggle-paste-state")) {
			if (CCNode* buttons = getChildByID("editor-buttons-menu")) {
				if (CCMenuItemSpriteExtra* pasteStateButton = typeinfo_cast<CCMenuItemSpriteExtra*>(buttons->getChildByID("paste-state-button"))) {
					m_fields->m_pasteStateCallback = pasteStateButton->m_pfnSelector;
					m_fields->m_pasteStateTarget = pasteStateButton->m_pListener;
					pasteStateButton->m_pfnSelector = menu_selector(MyEditorUI::onBetterPasteState);
				}
			}
		}
		if (Mod::get()->getSettingValue<bool>("toggle-paste-color")) {
			if (CCNode* buttons = getChildByID("editor-buttons-menu")) {
				if (CCMenuItemSpriteExtra* pasteColorButton = typeinfo_cast<CCMenuItemSpriteExtra*>(buttons->getChildByID("paste-color-button"))) {
					m_fields->m_pasteColorCallback = pasteColorButton->m_pfnSelector;
					m_fields->m_pasteColorTarget = pasteColorButton->m_pListener;
					pasteColorButton->m_pfnSelector = menu_selector(MyEditorUI::onBetterPasteColor);
				}
			}
		}
		return true;
	}

	void onBetterPasteState(CCObject* obj) {
		bool always = Mod::get()->getSettingValue<bool>("always-show");
		if ((always && m_selectedObject) || (!always && m_selectedObjects && m_selectedObjects->count() > 1)) {
			geode::createQuickPopup("Paste State?", "Pasting state is <cr>dangerous</c>! Are you sure?", "Cancel", "Yes", [this, obj] (FLAlertLayer*, bool yes) {
				if (yes) (m_fields->m_pasteStateTarget->*(m_fields->m_pasteStateCallback))(obj);
			}, true, true);
		} 
		else {
			(m_fields->m_pasteStateTarget->*(m_fields->m_pasteStateCallback))(obj);
		}
	}
	void onBetterPasteColor(CCObject* obj) {
		bool always = Mod::get()->getSettingValue<bool>("always-show");
		if ((always && m_selectedObject) || (!always && m_selectedObjects && m_selectedObjects->count() > 1)) {
			geode::createQuickPopup("Paste Color?", "Pasting color is <cr>dangerous</c>! Are you sure?", "Cancel", "Yes", [this, obj] (FLAlertLayer*, bool yes) {
				if (yes) (m_fields->m_pasteColorTarget->*(m_fields->m_pasteColorCallback))(obj);
			}, true, true);
		} 
		else {
			(m_fields->m_pasteColorTarget->*(m_fields->m_pasteColorCallback))(obj);
		}
	}
};

class $modify(MySetGroupIDLayer, SetGroupIDLayer) {

	struct Fields {
		SEL_MenuHandler m_pasteCallback;
		CCObject* m_pasteTarget;
	};

    bool init(GameObject* obj, cocos2d::CCArray* objs) {
		if (!SetGroupIDLayer::init(obj, objs)) return false;

		if (Mod::get()->getSettingValue<bool>("toggle-paste-state-group")) {
			if (CCNode* actions = m_mainLayer->getChildByID("actions-menu")) {
				if (CCMenuItemSpriteExtra* pasteButton = typeinfo_cast<CCMenuItemSpriteExtra*>(actions->getChildByID("paste-button"))) {
					m_fields->m_pasteCallback = pasteButton->m_pfnSelector;
					m_fields->m_pasteTarget = pasteButton->m_pListener;
					pasteButton->m_pfnSelector = menu_selector(MySetGroupIDLayer::onBetterPaste);
				}
			}
		}
		return true;
	}

	void onBetterPaste(CCObject* obj) {
		EditorUI* editorUI = EditorUI::get();

		bool always = Mod::get()->getSettingValue<bool>("always-show");
		if ((always && editorUI->m_selectedObject) || (!always && editorUI->m_selectedObjects && editorUI->m_selectedObjects->count() > 1)) {
			geode::createQuickPopup("Paste State?", "Pasting state is <cr>dangerous</c>! Are you sure?", "Cancel", "Yes", [this, obj] (FLAlertLayer*, bool yes) {
				if (yes) (m_fields->m_pasteTarget->*(m_fields->m_pasteCallback))(obj);
			}, true, true);
		} 
		else {
			(m_fields->m_pasteTarget->*(m_fields->m_pasteCallback))(obj);
		}
	}
};