#include <cassert>

#include "Input.h"

namespace JEngine
{
	MouseState Input::mouseStatesA[NUM_MOUSE_BUTTONS];
	MouseState Input::mouseStatesB[NUM_MOUSE_BUTTONS];
	KeyState Input::keyStatesA[NUM_KEYS];
	KeyState Input::keyStatesB[NUM_KEYS];
	bool Input::statesAreA = false;
	dvec2 Input::mousePosition;

	std::vector<std::shared_ptr<IKeyboardListener>> Input::keyboardListeners;
	std::vector<std::shared_ptr<IMouseListener>> Input::mouseListeners;


	void Input::setCallbackFunctions(GLFWwindow * _window)
	{
		glfwSetKeyCallback(_window, Input::keyCallback);
		glfwSetCursorPosCallback(_window, mousePosCallback);
	}

	KeyState Input::isKeyDown(const int _key)
	{
		assert(0 <= _key);
		assert(_key < 255);

		return statesAreA ? keyStatesA[_key] : keyStatesB[_key];
	}

	MouseState Input::isMouseDown(const int _button)
	{
		assert(0 <= _button);
		assert(_button < 3);

		return statesAreA ? mouseStatesA[_button] : mouseStatesB[_button];
	}

	dvec2 Input::getMousePosition()
	{
		return mousePosition;
	}

	void Input::update()
	{
		//Alternate current key state array
		statesAreA = !statesAreA;
	}

	bool Input::initialise(GLFWwindow * _window)
	{
		for (int i = 0; i < NUM_KEYS; ++i)
		{
			keyStatesA[i] = KeyState::KEY_RELEASED;
			keyStatesB[i] = KeyState::KEY_RELEASED;
		}

		for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
		{
			mouseStatesA[i] = MouseState::MOUSE_RELEASED;
			mouseStatesB[i] = MouseState::MOUSE_RELEASED;
		}

		Input::setCallbackFunctions(_window);

		return true;
	}

	void Input::keyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods)
	{
		assert(0 <= _key);
		assert(_key < NUM_KEYS);

		auto keyStates = statesAreA ? keyStatesA : keyStatesB;

		if (_action == GLFW_RELEASE)
		{
			keyStates[_key] = KeyState::KEY_RELEASED;

			for (const auto & keyboardListener : keyboardListeners)
				keyboardListener->keyUp(_key);
		}
		else if (_action == GLFW_PRESS)
		{
			keyStates[_key] = KeyState::KEY_PRESSED;

			for (const auto & keyboardListener : keyboardListeners)
				keyboardListener->keyDown(_key);
		}
		else if (_action == GLFW_REPEAT)
		{
			keyStates[_key] = KeyState::KEY_PRESSED;

			for (const auto & keyboardListener : keyboardListeners)
				keyboardListener->keyDown(_key);
		}
	}

	void Input::mousePosCallback(GLFWwindow * _window, double _x, double _y)
	{
		mousePosition.set_xy(_x, _y);
	}

	void Input::mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods)
	{
		assert(0 <= _button);
		assert(_button < NUM_MOUSE_BUTTONS);

		auto mouseStates = statesAreA ? mouseStatesA : mouseStatesB;

		if (_action == GLFW_RELEASE)
		{
			mouseStates[_button] = MouseState::MOUSE_RELEASED;

			for (const auto & mouseListener : mouseListeners)
				mouseListener->mouseUp(_button);
		}
		else if (_action == GLFW_PRESS)
		{
			mouseStates[_button] = MouseState::MOUSE_PRESSED;

			for (const auto & mouseListener : mouseListeners)
				mouseListener->mouseDown(_button);
		}
	}
}