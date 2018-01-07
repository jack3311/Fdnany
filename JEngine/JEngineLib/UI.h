#pragma once

#include <memory>
#include <vector>

#include "JEvent.h"
#include "Renderer.h"
#include "Maths.h"

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
		virtual void render() const;
		virtual void update();
	};

	class UIPanel : public UIElement
	{
	public:
		std::vector<std::shared_ptr<UIElement>> elements;

		void setActive(bool);

		void render() const;
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

		void render() const;
	};

	class UILabel : public UIElement
	{
	public:
		std::string text;

		UILabel(std::string);

		void render() const;
	};

	struct UITexturedVertexFormat
	{
		fvec2 position;
		fvec2 texCoords;

		static void setupVertexAttributes();

		UITexturedVertexFormat(fvec2, fvec2);
	};

	class UI
	{
	private:
		std::unique_ptr<UIPanelSwitcher> uiBase;
		std::shared_ptr<UIPanel> uiDebug;

		std::unique_ptr<Renderer<UITexturedVertexFormat, true>> texturedRectRenderer;

		bool setupDebugUI();
		void updateDebugUI();

		struct {
			std::shared_ptr<UILabel> fps;
			std::shared_ptr<UILabel> spf;
		} uiDebugTable;

	public:
		UI();
		~UI();

		void render() const;
		void update();

		bool initialise();

		UIPanelSwitcher & getUIBase();
	};
}