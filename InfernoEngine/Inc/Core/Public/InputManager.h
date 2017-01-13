#pragma once

#include "InputVals.h"
#include "Globals.h"
#include "DynArray.h"

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <set>
#include <unordered_map>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

class Input
{
private:
	typedef void(*InputFunction)(uint32_t);

	enum BUTTON_STATES { DOWN = 3, PRESSED = 2, RELEASED = 1, UP = 0, TOTAL_STATES };
	enum FLAGS { CONTROLLER_1 = 1, CONTROLLER_2 = 2, CONTROLLER_3 = 4, CONTROLLER_4 = 8, CENTER_MOUSE = 16, CHECK_ALL_KEYS = 32 };

	static XINPUT_STATE s_ControllerInputStates[XUSER_MAX_COUNT];
	static char s_keys[TOTAL_KEYS];
	static char s_changes[TOTAL_KEYS];
	static float s_axisValues[XUSER_MAX_COUNT][TOTAL_AXIS];
	static char s_buttonStates[XUSER_MAX_COUNT][TOTAL_BUTTONS];
	static InputFunction s_pKeyFunctions[TOTAL_STATES];
	static InputFunction s_pMouseFunctions[TOTAL_STATES];
	static InputFunction s_pControllerFunctions[TOTAL_STATES];
	static std::set<uint32_t> s_setRegisteredKeys;
	static HWND s_hWindow;
	static DynArray<uint32_t> s_aConnectedControllers;
	static POINT s_WindowPosition;
	static DynArray<uint32_t> s_aDisconnectedControllers;
	static POINT s_prevMousePosition;
	static POINT s_mouseMovement;
	static uint32_t s_iMouseWheel;
	static uint32_t s_iMouseWheelChange;
	static SHORT s_ThumbDeadZone;
	static BYTE s_TriggerThreshold;
	static char s_cMouseButtons;
	static char s_cMouseButtonStates[TOTAL_MOUSE_BUTTONS];
	static char s_cFlags;
	static bool s_bLostFocus;
	static bool s_bShowMouse;
	static bool s_bUsingController;
	static bool s_bPrevUsingController;

	// IN: int - key that was pressed
	// OUT: void
	//
	// Sets the key passed in as down
	static void KeyDown(int key);
	// IN: int - key that was released
	// OUT: void
	//
	// Sets the key passed in as up
	static void KeyUp(int key);
	// IN: int - mouse button that was pressed
	// OUT: void
	//
	// Sets the mouse button passed in as down
	static void MouseButtonDown(int mbutton);
	// IN: int - mouse button that was released
	// OUT: void
	//
	// Sets the mouse button passed in as up
	static void MouseButtonUp(int mbutton);
	// IN: int - mouse wheel direction
	// OUT: void
	//
	// Sets the mouse wheel direction for this frame
	static void MouseWheelDirection(int mouseWheel);
	// IN: int - window x position
	// IN: int - window y position
	// OUT: void
	//
	// Updates the windows top left position
	static void UpdateWindowPosition(int x, int y);
	// IN: bool - Lost Window Focus
	// OUT: void
	//
	// Tells the InputManager if we lost window focus
	static void LostWindowFocus(bool lostFocus);
	// IN: void
	// OUT: void
	//
	// Notifying call made when a device change was made
	static void DeviceChange();

public:
	// IN: HWND - handle to the window that will be used by the input manager.
	//            Only needed for centering purposes so nullptr can be passed and it will use the desktop
	// OUT: bool - States if the manager set up correctly
	//
	// Initializes the input manager
	static bool Initialize(HWND window);

	// IN: void
	// OUT: bool - Status of if the controller is being used
	//
	// Returns the status of the use of the controller
	static bool UsingController();

	// IN: Key - Key to register
	// OUT: void
	//
	// Registers the key if it has not already been registered
	static void RegisterKey(Key key);
	// IN: Key - Key to unregister
	// OUT: void
	//
	// Unregisters the key if it is registered
	static void UnregisterKey(Key key);

	// IN: void
	// OUT: bool - States if the manager updated successfully
	//
	// Updates the input manager from all changes from last frame
	static bool Update();

	// IN: Button       - Button to check
	// OUT: bool - Status of the button
	//
	// Checks the button to see if it was pressed this frame
	static bool IsMouseButtonPressed(Button button);
	// IN: Button       - Button to check
	// OUT: bool - Status of the button
	//
	// Checks the button to see if it is down
	static bool IsMouseButtonDown(Button button);
	// IN: Button       - Button to check
	// OUT: bool - Status of the button
	//
	// Checks the button to see if it was released this frame
	static bool IsMouseButtonReleased(Button button);

	// IN: void
	// OUT: bool - States if any button was pressed
	//
	// Checks to see if any button has been pressed this frame
	static bool IsAnyMouseButtonPressed();
	// IN: void
	// OUT: bool - States if any button is down
	//
	// Checks to see if any button is currently down
	static bool IsAnyMouseButtonDown();
	// IN: void
	// OUT: bool - States if any button was released
	//
	// Checks to see if any button has been released this frame
	static bool IsAnyMouseButtonReleased();

	// IN: Key - Key to check
	// OUT: bool - Status of the key
	//
	// Checks the key to see if it was pressed this frame
	static bool IsKeyPressed(Key key);
	// IN: Key - Key to check
	// OUT: bool - Status of the key
	//
	// Checks the key to see if it is down
	static bool IsKeyDown(Key key);
	// IN: Key - Key to check
	// OUT: bool - Status of the key
	//
	// Checks the key to see if it was released this frame
	static bool IsKeyReleased(Key key);

	// IN: void
	// OUT: bool - States if any key was pressed
	//
	// Checks to see if any key has been pressed this frame
	static bool IsAnyKeyPressed();
	// IN: void
	// OUT: bool - States if any key is down
	//
	// Checks to see if any key is currently down
	static bool IsAnyKeyDown();
	// IN: void
	// OUT: bool - States if any key was released
	//
	// Checks to see if any key has been released this frame
	static bool IsAnyKeyReleased();

	// IN: unsigned int - Controller to check 
	//     Button       - Button to check
	// OUT: bool - Status of the button
	//
	// Checks the button to see if it was pressed this frame
	static bool IsButtonPressed(unsigned int controller, Button button);
	// IN: unsigned int - Controller to check 
	//     Button       - Button to check
	// OUT: bool - Status of the button
	//
	// Checks the button to see if it is down
	static bool IsButtonDown(unsigned int controller, Button button);
	// IN: unsigned int - Controller to check 
	//     Button       - Button to check
	// OUT: bool - Status of the button
	//
	// Checks the button to see if it was released this frame
	static bool IsButtonReleased(unsigned int controller, Button button);

	// IN: unsigned int - Controller to check 
	// OUT: bool - States if any button was pressed
	//
	// Checks to see if any button has been pressed this frame
	static bool IsAnyButtonPressed(unsigned int controller);
	// IN: unsigned int - Controller to check 
	// OUT: bool - States if any button is down
	//
	// Checks to see if any button is currently down
	static bool IsAnyButtonDown(unsigned int controller);
	// IN: unsigned int - Controller to check 
	// OUT: bool - States if any button was released
	//
	// Checks to see if any button has been released this frame
	static bool IsAnyButtonReleased(unsigned int controller);

	// IN: unsigned int - Controller to check
	//     Axis         - Axis to check
	// OUT: int - Range of the axis
	//
	// Checks the axis of the controller specified and returns the value
	static float GetAxis(unsigned int controller, Axis axis);

	// IN: void
	// OUT: int - MouseWheel Direction
	//
	// Returns the direction of the mouse wheel this frame
	// 1 - Wheel Up, 0 - No Movement, -1 - Wheel Down
	static int GetMouseWheel();

	// IN: HWND - Window to use by the manager
	// OUT: void
	//
	// Sets the incoming window as the window to use for centering the mouse
	static void SetWindow(HWND window);
	// IN: SHORT - DeadZone to use
	// OUT: void
	//
	// Sets the deadzone to the specified amount if in range
	static void SetThumbDeadZone(SHORT deadzone);
	// IN: BYTE - Threshold to use
	// OUT: void
	//
	// Sets the threshold to the specified amount if in range
	static void SetTriggerThreshold(BYTE threshold);
	// IN: bool - States the visibilty of the mouse
	// OUT: void
	//
	// Sets the mouse's visibility based on the incoming bool
	static void ShowMouse(bool visible);
	// IN: void
	// OUT: POINT - Movement made by the mouse
	//
	// Returns the total movement the mouse has made since last frame
	static POINT GetMouseMovement();
	// IN: void
	// OUT: POINT - Current mouse position on screen
	//
	// Returns the current mouse position on screen
	static POINT GetMousePosition();

	friend class InfernoEngine;
};

