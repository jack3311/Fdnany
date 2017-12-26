#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <vector>
#include <memory>

#include "Maths.h"

#include "IInputListeners.h"

namespace JEngine
{

#define NUM_KEYS 348
#define NUM_MOUSE_BUTTONS 8

	enum class MouseButton : __int8
	{
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT
	};

	enum class KeyState : __int8
	{
		KEY_RELEASED,
		KEY_PRESSED
	};

	enum class MouseState : __int8
	{
		MOUSE_RELEASED,
		MOUSE_PRESSED
	};

	class Input
	{
	public:

		/// 
		/// setCallbackFunctions:
		/// Sets up keyboard and mouse input with OpenGL
		/// _window - a pointer to the relevant window
		///
		static void setCallbackFunctions(GLFWwindow * _window);

		/// 
		/// isKeyDown:
		/// returns whether a given key is currently pressed
		/// _key - the key
		///
		static KeyState isKeyDown(const int _key);

		/// 
		/// isMouseDown:
		/// returns whether a given mouse button is currently pressed
		/// _button - the mouse button
		///
		static MouseState isMouseDown(const int _button);

		/// 
		/// getMousePosition:
		/// returns the current mouse position in screen coordinates
		///
		static dvec2 getMousePosition();

		///
		/// update:
		/// updates input information
		///
		static void update();

		///
		/// initialise:
		/// initialises input
		/// _window: a pointer to the relevant window
		///
		static bool initialise(GLFWwindow * _window);

	private:
		Input() = delete;
		Input(const Input&) = delete;
		~Input() = delete;

		static dvec2 mousePosition;
		static bool statesAreA;
		static MouseState mouseStatesA[NUM_MOUSE_BUTTONS];
		static MouseState mouseStatesB[NUM_MOUSE_BUTTONS];
		static KeyState keyStatesA[NUM_KEYS];
		static KeyState keyStatesB[NUM_KEYS];

		static std::vector<std::shared_ptr<IKeyboardListener>> keyboardListeners;
		static std::vector<std::shared_ptr<IMouseListener>> mouseListeners;

		static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
		static void mousePosCallback(GLFWwindow * _window, double _xpos, double _ypos);
		void mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods);
	};

}