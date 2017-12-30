#include "UI.h"

#include <cassert>

namespace JEngine
{
	UI::UI()
	{
		uiBase = std::make_unique<UIPanelSwitcher>();
	}

	UI::~UI()
	{
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

	void UIElement::render()
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

	void UIPanel::setActive(bool _active)
	{
		for (auto & element : elements)
		{
			element->setActive(_active);
		}

		UIElement::setActive(_active);
	}
}