#include "InputManager.h"
#include "AssertHelper.h"
#include <string>

#define MAX_DEADZONE 32767
#define MAX_DEADZONE_NEG 32768
#define MAX_THRESHOLD 255

XINPUT_STATE Input::s_ControllerInputStates[XUSER_MAX_COUNT];
char Input::s_keys[TOTAL_KEYS];
char Input::s_changes[TOTAL_KEYS];
float Input::s_axisValues[XUSER_MAX_COUNT][TOTAL_AXIS];
char Input::s_buttonStates[XUSER_MAX_COUNT][TOTAL_BUTTONS];
Input::InputFunction Input::s_pKeyFunctions[TOTAL_STATES];
Input::InputFunction Input::s_pMouseFunctions[TOTAL_STATES];
Input::InputFunction Input::s_pControllerFunctions[TOTAL_STATES];
std::set<uint32_t> Input::s_setRegisteredKeys;
HWND Input::s_hWindow;
DynArray<uint32_t> Input::s_aConnectedControllers(XUSER_MAX_COUNT);
POINT Input::s_WindowPosition;
DynArray<uint32_t> Input::s_aDisconnectedControllers(XUSER_MAX_COUNT);
POINT Input::s_prevMousePosition;
POINT Input::s_mouseMovement;
uint32_t Input::s_iMouseWheel;
uint32_t Input::s_iMouseWheelChange;
SHORT Input::s_ThumbDeadZone;
BYTE Input::s_TriggerThreshold;
char Input::s_cMouseButtons;
char Input::s_cMouseButtonStates[TOTAL_MOUSE_BUTTONS];
char Input::s_cFlags;
bool Input::s_bLostFocus;
bool Input::s_bShowMouse;
bool Input::s_bUsingController;
bool Input::s_bPrevUsingController;

// Private Methods
// --------------------------------------

// IN: int - key that was pressed
// OUT: void
//
// Sets the key passed in as down
void Input::KeyDown(int key) { s_changes[key] = PRESSED; }
// IN: int - key that was released
// OUT: void
//
// Sets the key passed in as up
void Input::KeyUp(int key) { s_changes[key] = UP; }
// IN: int - mouse button that was pressed
// OUT: void
//
// Sets the mouse button passed in as down
void Input::MouseButtonDown(int mbutton){ s_cMouseButtons |= (1 << mbutton); }
// IN: int - mouse button that was released
// OUT: void
//
// Sets the mouse button passed in as up
void Input::MouseButtonUp(int mbutton) { s_cMouseButtons &= ~(1 << mbutton); }
// IN: int - mouse wheel direction
// OUT: void
//
// Sets the mouse wheel direction for this frame
void Input::MouseWheelDirection(int mouseWheel) { s_iMouseWheelChange = mouseWheel; }
// IN: int - window x position
// IN: int - window y position
// OUT: void
//
// Updates the windows top left position
void Input::UpdateWindowPosition(int x, int y)
{
	s_WindowPosition.x = x;
	s_WindowPosition.y = y;
}
// IN: bool - Lost Window Focus
// OUT: void
//
// Tells the InputManager if we lost window focus
void Input::LostWindowFocus(bool lostFocus)
{
	ShowMouse(s_bLostFocus = lostFocus ? true : s_bShowMouse); // Assignment is done purposely
}
// IN: void
// OUT: void
//
// Notifying call made when a USB device is connected
void Input::DeviceChange()
{
	DWORD dwResult;

	for (uint32_t i = 0, size = s_aConnectedControllers.Size(); i < size; i++)
	{
		uint32_t controllerIndex = s_aConnectedControllers[i];
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(controllerIndex, &s_ControllerInputStates[controllerIndex]);

		// If we can no longer ping the controller it is disconnected
		if (dwResult != ERROR_SUCCESS)
		{
			s_aDisconnectedControllers.push_back(controllerIndex);
			s_aConnectedControllers.Remove(i);

			std::string print = "Controller " + std::to_string(controllerIndex + 1) + " Disconnected!\n";
			PRINT(print.c_str());
		}
	}

	for (uint32_t i = 0, size = s_aDisconnectedControllers.Size(); i < size; i++)
	{
		uint32_t controllerIndex = s_aDisconnectedControllers[i];
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(controllerIndex, &s_ControllerInputStates[controllerIndex]);
		
		// If we successfully pinged the controller it is connected
		if (dwResult == ERROR_SUCCESS)
		{
			s_aConnectedControllers.push_back(controllerIndex);
			s_aDisconnectedControllers.Remove(i);

			std::string print = "Controller " + std::to_string(controllerIndex + 1) + " Connected!\n";
			PRINT(print.c_str());
		}
	}
}

// Public Methods
// ----------------------------------------------------
// IN: HWND - handle to the window that will be used by the input manager.
//            Only needed for centering purposes so nullptr can be passed and it will use the desktop
// OUT: bool - States if the manager set up correctly
//
// Initializes the input manager
bool Input::Initialize(HWND window)
{
	// Sets all our keys to 0 by default
	ZeroMemory(s_keys, TOTAL_KEYS);

	s_hWindow = window;
	GetCursorPos(&s_prevMousePosition);
	s_mouseMovement.x = s_mouseMovement.y = 0;
	s_iMouseWheel = s_iMouseWheelChange = 0;
	s_cFlags = 0;

	// Do the initial check to see which controllers are connected
	DWORD dwResult;
	for (DWORD i = 0; i< XUSER_MAX_COUNT; i++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &s_ControllerInputStates[i]);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
			s_cFlags |= CONTROLLER_1 << i;
			s_aConnectedControllers.push_back(i);

			std::string print = "Controller " + std::to_string(i + 1) + " Connected!\n";
			PRINT(print.c_str());
		}
		else
			s_aDisconnectedControllers.push_back(i);
	}

	return true;
}

// IN: void
// OUT: bool - Status of if the controller is being used
//
// Returns the status of the use of the controller
bool Input::UsingController() { return s_bUsingController; }

// IN: Key - Key to register
// OUT: void
//
// Registers the key if it has not already been registered
void Input::RegisterKey(Key key) { s_setRegisteredKeys.insert((uint32_t)key); }
// IN: Key - Key to unregister
// OUT: void
//
// Unregisters the key if it is registered
void Input::UnregisterKey(Key key) { s_setRegisteredKeys.erase((uint32_t)key); }

// IN: void
// OUT: bool - States if the manager updated successfully
//
// Updates the input manager from all changes from last frame
bool Input::Update()
{
	// TODO::

	return true;
}

// IN: Button       - Button to check
// OUT: bool - Status of the button
//
// Checks the button to see if it was pressed this frame
bool Input::IsMouseButtonPressed(Button button) { return (button == Button::None) ? false : s_cMouseButtonStates[(uint32_t)button] == PRESSED; }
// IN: Button       - Button to check
// OUT: bool - Status of the button
//
// Checks the button to see if it is down
bool Input::IsMouseButtonDown(Button button) { return (button == Button::None) ? false : s_cMouseButtonStates[(uint32_t)button] == DOWN; }
// IN: Button       - Button to check
// OUT: bool - Status of the button
//
// Checks the button to see if it was released this frame
bool Input::IsMouseButtonReleased(Button button)  { return (button == Button::None) ? false : s_cMouseButtonStates[(uint32_t)button] == RELEASED; }

// IN: void
// OUT: bool - States if any button was pressed
//
// Checks to see if any button has been pressed this frame
bool Input::IsAnyMouseButtonPressed()
{
	for (uint32_t button = 0; button < TOTAL_MOUSE_BUTTONS; button++)
		if (s_cMouseButtonStates[button] == PRESSED)
			return true;

	return false;
}
// IN: void
// OUT: bool - States if any button is down
//
// Checks to see if any button is currently down
bool Input::IsAnyMouseButtonDown()
{
	for (uint32_t button = 0; button < TOTAL_MOUSE_BUTTONS; button++)
		if (s_cMouseButtonStates[button] == DOWN)
			return true;

	return false;
}
// IN: void
// OUT: bool - States if any button was released
//
// Checks to see if any button has been released this frame
bool Input::IsAnyMouseButtonReleased()
{
	for (uint32_t button = 0; button < TOTAL_MOUSE_BUTTONS; button++)
		if (s_cMouseButtonStates[button] == RELEASED)
			return true;

	return false;
}

// IN: Key - Key to check
// OUT: bool - Status of the key
//
// Checks the key to see if it was pressed this frame
bool Input::IsKeyPressed(Key key) { return (key == Key::None) ? false : s_keys[(uint32_t)key] == PRESSED; }
// IN: Key - Key to check
// OUT: bool - Status of the key
//
// Checks the key to see if it is down
bool Input::IsKeyDown(Key key) { return (key == Key::None) ? false : s_keys[(uint32_t)key] == DOWN; }
// IN: Key - Key to check
// OUT: bool - Status of the key
//
// Checks the key to see if it was released this frame
bool Input::IsKeyReleased(Key key) { return (key == Key::None) ? false : s_keys[(uint32_t)key] == RELEASED; }

// IN: void
// OUT: bool - States if any key was pressed
//
// Checks to see if any key has been pressed this frame
bool Input::IsAnyKeyPressed()
{
	for (uint32_t key = 0; key < TOTAL_KEYS; key++)
		if (s_keys[key] == PRESSED)
			return true;

	return false;
}
// IN: void
// OUT: bool - States if any key is down
//
// Checks to see if any key is currently down
bool Input::IsAnyKeyDown()
{
	for (uint32_t key = 0; key < TOTAL_KEYS; key++)
		if (s_keys[key] == DOWN)
			return true;

	return false;
}
// IN: void
// OUT: bool - States if any key was released
//
// Checks to see if any key has been released this frame
bool Input::IsAnyKeyReleased()
{
	for (uint32_t key = 0; key < TOTAL_KEYS; key++)
		if (s_keys[key] == RELEASED)
			return true;

	return false;
}

// IN: unsigned int - Controller to check 
//     Button       - Button to check
// OUT: bool - Status of the button
//
// Checks the button to see if it was pressed this frame
bool Input::IsButtonPressed(unsigned int controller, Button button)
{ return (controller > XUSER_MAX_COUNT || !(s_cFlags & (CONTROLLER_1 << controller)) || button == Button::None) ? false : s_buttonStates[controller][(uint32_t)button] == PRESSED; }
// IN: unsigned int - Controller to check 
//     Button       - Button to check
// OUT: bool - Status of the button
//
// Checks the button to see if it is down
bool Input::IsButtonDown(unsigned int controller, Button button)
{ return (controller > XUSER_MAX_COUNT || !(s_cFlags & (CONTROLLER_1 << controller)) || button == Button::None) ? false : s_buttonStates[controller][(uint32_t)button] == DOWN; }
// IN: unsigned int - Controller to check 
//     Button       - Button to check
// OUT: bool - Status of the button
//
// Checks the button to see if it was released this frame
bool Input::IsButtonReleased(unsigned int controller, Button button)
{ return (controller > XUSER_MAX_COUNT || !(s_cFlags & (CONTROLLER_1 << controller)) ||  button == Button::None) ? false : s_buttonStates[controller][(uint32_t)button] == RELEASED; }

// IN: unsigned int - Controller to check 
// OUT: bool - States if any button was pressed
//
// Checks to see if any button has been pressed this frame
bool Input::IsAnyButtonPressed(unsigned int controller)
{
	if (controller > XUSER_MAX_COUNT || !(s_cFlags & (CONTROLLER_1 << controller)))
		return false;

	for (uint32_t button = 0; button < TOTAL_BUTTONS; button++)
		if (s_buttonStates[controller][button] == PRESSED)
			return true;

	return false;
}
// IN: unsigned int - Controller to check 
// OUT: bool - States if any button is down
//
// Checks to see if any button is currently down
bool Input::IsAnyButtonDown(unsigned int controller)
{
	if (controller > XUSER_MAX_COUNT || !(s_cFlags & (CONTROLLER_1 << controller)))
		return false;

	for (uint32_t button = 0; button < TOTAL_BUTTONS; button++)
		if (s_buttonStates[controller][button] == DOWN)
			return true;

	return false;
}
// IN: unsigned int - Controller to check 
// OUT: bool - States if any button was released
//
// Checks to see if any button has been released this frame
bool Input::IsAnyButtonReleased(unsigned int controller)
{
	if (controller > XUSER_MAX_COUNT || !(s_cFlags & (CONTROLLER_1 << controller)))
		return false;

	for (uint32_t button = 0; button < TOTAL_BUTTONS; button++)
		if (s_buttonStates[controller][button] == RELEASED)
			return true;

	return false;
}

// IN: unsigned int - Controller to check
//     Axis         - Axis to check
// OUT: int - Range of the axis
//
// Checks the axis of the controller specified and returns the value
float Input::GetAxis(unsigned int controller, Axis axis)
{
	if (controller > XUSER_MAX_COUNT || !(s_cFlags & (CONTROLLER_1 << controller)) || axis == Axis::None)
		return false;

	return s_axisValues[controller][(uint32_t)axis];
}
// IN: void
// OUT: int - MouseWheel Direction
//
// Returns the direction of the mouse wheel this frame
// 1 - Wheel Up, 0 - No Movement, -1 - Wheel Down
int Input::GetMouseWheel() { return s_iMouseWheel == 2 ? -1 : s_iMouseWheel; }

// IN: HWND - Window to use by the manager
// OUT: void
//
// Sets the incoming window as the window to use for centering the mouse
void Input::SetWindow(HWND window) { s_hWindow = window; }
// IN: SHORT - DeadZone to use
// OUT: void
//
// Sets the deadzone to the specified amount if in range
void Input::SetThumbDeadZone(SHORT deadzone) { s_ThumbDeadZone = deadzone > MAX_DEADZONE ? MAX_DEADZONE : deadzone; }
// IN: BYTE - Threshold to use
// OUT: void
//
// Sets the threshold to the specified amount if in range
void Input::SetTriggerThreshold(BYTE threshold) { s_TriggerThreshold = threshold;}
// IN: bool - States the visibilty of the mouse
// OUT: void
//
// Sets the mouse's visibility based on the incoming bool
void Input::ShowMouse(bool visible)
{
	if (!s_bLostFocus)
		s_bShowMouse = visible;

	if (visible)
		while (ShowCursor(visible) < 0);
	else
		while (ShowCursor(visible) >= 0);
}

// IN: void
// OUT: POINT - Movement made by the mouse
//
// Returns the total movement the mouse has made since last frame
POINT Input::GetMouseMovement() { return s_mouseMovement; }
// IN: void
// OUT: POINT - Current mouse position on screen
//
// Returns the current mouse position on screen
POINT Input::GetMousePosition() { return s_prevMousePosition; }