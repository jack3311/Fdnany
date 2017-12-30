#pragma once

#include <memory>
#include <vector>

#include "JEvent.h"

namespace JEngine
{
	class UIElement
	{
	public:
		UIElement();
		UIElement(const UIElement &) = delete;
		~UIElement();

		bool isActive;

		virtual void setActive(bool);
		void render();
		void update();
	};

	class UIPanel : public UIElement
	{
	public:
		std::vector<std::shared_ptr<UIElement>> elements;

		void setActive(bool);
	};
	
	class UIPanelSwitcher : public UIElement
	{
	private:
		std::vector<std::shared_ptr<UIPanel>> panels;
		std::shared_ptr<UIPanel> currentPanel;

	public:
		unsigned int addPanel(const std::shared_ptr<UIPanel> &);
		void setCurrentPanel(unsigned int);

		std::shared_ptr<UIPanel> getCurrentPanel();

		bool hasNoPanels() const;

		void setActive(bool);
	};

	class UI
	{
	private:
		std::unique_ptr<UIPanelSwitcher> uiBase;

	public:
		UI();
		~UI();

		UIPanelSwitcher & getUIBase();
	};
}