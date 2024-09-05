#include <Geode/Geode.hpp>
#include <Geode/modify/SetGroupIDLayer.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {

	struct Fields {
		SEL_MenuHandler m_pasteStateCallback;
		CCObject* m_pasteStateTarget;
	};

    bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;

		if (CCNode* buttons = getChildByID("editor-buttons-menu")) {
			if (CCMenuItemSpriteExtra* pasteStateButton = typeinfo_cast<CCMenuItemSpriteExtra*>(buttons->getChildByID("paste-state-button"))) {
				m_fields->m_pasteStateCallback = pasteStateButton->m_pfnSelector;
				m_fields->m_pasteStateTarget = pasteStateButton->m_pListener;
				pasteStateButton->m_pfnSelector = menu_selector(MyEditorUI::onBetterPasteState);
			}
		}
		return true;
	}

	void onBetterPasteState(CCObject* obj) {
		if (m_selectedObjects && m_selectedObjects->count() > 1){
			geode::createQuickPopup("Paste State?", "You have multiple objects selected, pasting state is <cr>dangerous</c>! Are you sure?", "Cancel", "Yes", [this, obj] (FLAlertLayer*, bool yes) {
				if (yes) (m_fields->m_pasteStateTarget->*(m_fields->m_pasteStateCallback))(obj);
			}, true, true);
		} 
		else {
			(m_fields->m_pasteStateTarget->*(m_fields->m_pasteStateCallback))(obj);
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

		if (CCNode* actions = m_mainLayer->getChildByID("actions-menu")) {
			if (CCMenuItemSpriteExtra* pasteButton = typeinfo_cast<CCMenuItemSpriteExtra*>(actions->getChildByID("paste-button"))) {
				m_fields->m_pasteCallback = pasteButton->m_pfnSelector;
				m_fields->m_pasteTarget = pasteButton->m_pListener;
				pasteButton->m_pfnSelector = menu_selector(MySetGroupIDLayer::onBetterPaste);
			}
		}
		return true;
	}

	void onBetterPaste(CCObject* obj) {

		EditorUI* editorUI = EditorUI::get();
		if (editorUI->m_selectedObjects && editorUI->m_selectedObjects->count() > 1){
			geode::createQuickPopup("Paste State?", "You have multiple objects selected, pasting state is <cr>dangerous</c>! Are you sure?", "Cancel", "Yes", [this, obj] (FLAlertLayer*, bool yes) {
				if (yes) (m_fields->m_pasteTarget->*(m_fields->m_pasteCallback))(obj);
			}, true, true);
		} 
		else {
			(m_fields->m_pasteTarget->*(m_fields->m_pasteCallback))(obj);
		}
	}
};