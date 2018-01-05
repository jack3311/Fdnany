#include "UI.h"

#include "Logger.h"
#include "Util.h"
#include "EngineTime.h"
#include "Engine.h"

#include <cassert>

namespace JEngine
{
	bool UI::setupDebugUI()
	{
		//s/f, f/s
		uiDebug->elements.push_back(uiDebugTable.spf = std::make_shared<UILabel>("s/f: "));
		uiDebug->elements.push_back(uiDebugTable.fps = std::make_shared<UILabel>("f/s: "));

		return true;
	}

	void UI::updateDebugUI()
	{
		//s/f, f/s
		uiDebugTable.spf->text = toString(Engine::getEngine().getEngineTime().getSpf());
		uiDebugTable.fps->text = toString(Engine::getEngine().getEngineTime().getFps());
	}

	UI::UI()
	{
		uiBase = std::make_unique<UIPanelSwitcher>();
		uiDebug = std::make_shared<UIPanel>();
	}

	UI::~UI()
	{
	}

	void UI::render() const
	{
		uiBase->render();
		uiDebug->render();
	}

	void UI::update()
	{
		uiBase->update();
		uiDebug->update();

		updateDebugUI();
	}

	bool UI::initialise()
	{
		ERR_IF(!setupDebugUI(), "Could not set up debug UI");

		return true;
	}

	UIPanelSwitcher & UI::getUIBase()
	{
		return *uiBase;
	}

	UIElement::UIElement()
	{
	}

	UIElement::~UIElement()
	{
	}

	void UIElement::setActive(bool _active)
	{
		isActive = _active;
	}

	void UIElement::render() const
	{
		
	}

	void UIElement::update()
	{
	}

	unsigned int UIPanelSwitcher::addPanel(const std::shared_ptr<UIPanel> & _panel)
	{
		panels.push_back(_panel);

		return static_cast<unsigned int>(panels.size()) - 1u;
	}

	void UIPanelSwitcher::setCurrentPanel(unsigned int _panel)
	{
		assert(0u <= _panel);
		assert(_panel < panels.size());

		for (auto & panel : panels)
		{
			panel->setActive(false);
		}
		
		currentPanel = panels[_panel];

		currentPanel->setActive(isActive);
	}

	std::shared_ptr<UIPanel> UIPanelSwitcher::getCurrentPanel()
	{
		return currentPanel;
	}

	bool UIPanelSwitcher::hasNoPanels() const
	{
		return panels.empty();
	}

	void UIPanelSwitcher::setActive(bool _active)
	{
		currentPanel->setActive(_active);

		UIElement::setActive(_active);
	}

	void UIPanelSwitcher::render() const
	{
		if (currentPanel)
		{
			currentPanel->render();
		}
	}

	void UIPanel::setActive(bool _active)
	{
		for (auto & element : elements)
		{
			element->setActive(_active);
		}

		UIElement::setActive(_active);
	}

	void UIPanel::render() const
	{
		for (auto & element : elements)
		{
			element->render();
		}
	}

	UILabel::UILabel(std::string _text) : text(_text), UIElement()
	{
	}
	
	void UILabel::render() const
	{
		Logger::getLogger().log(text);

		UIElement::render();
	}
}