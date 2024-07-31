// JoystickToMouse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <math.h>
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

// Function to simulate mouse movement
void MoveMouse(int x, int y) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = x;
    input.mi.dy = y;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
}

// Function to simulate mouse button press
void MouseClick(DWORD button) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = button;
    SendInput(1, &input, sizeof(INPUT));
}

int main() {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    int sensitivity = 5;
    const float sensitivityAnalog = 0.2f;

    const SHORT minDeadzone = 3000; // Minimum deadzone
    const SHORT maxDeadzone = 32767; // Maximum deadzone (you can adjust this if needed)

    bool wasAButtonPressed = false; // To track the previous state of the A button
    bool wasBButtonPressed = false; // To track the previous state of the B button

    bool wasLBButtonPressed = false; // To track the previous state of the A button
    bool wasRBButtonPressed = false; // To track the previous state of the B button


    while (true) {

        // Get the state of the controller from XInput
        if (XInputGetState(0, &state) == ERROR_SUCCESS) {
            // Controller is connected
            SHORT lx = state.Gamepad.sThumbLX;
            SHORT ly = state.Gamepad.sThumbLY;

            bool isAButtonPressed = state.Gamepad.wButtons & XINPUT_GAMEPAD_A;
            bool isBButtonPressed = state.Gamepad.wButtons & XINPUT_GAMEPAD_B;

            bool is_LB_ButtonPressed = state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
            bool is_RB_ButtonPressed = state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

            bool dPadUP_ButtonPressed = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
            bool dPadDOWN_ButtonPressed = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
            bool dPadLEFT_ButtonPressed = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
            bool dPadRIGHT_ButtonPressed = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

            // Deadzone check for the left thumbstick
            if (abs(lx) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || abs(ly) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                int mouseX = static_cast<int>(lx * sensitivityAnalog) / 1000; // Adjust the divisor for sensitivity
                int mouseY = static_cast<int>(-ly * sensitivityAnalog) / 1000; // Invert Y-axis

                // Move the mouse
                MoveMouse(mouseX, mouseY);
            }

            // Dpad
            if (dPadUP_ButtonPressed) {
                MoveMouse(0, -1 * sensitivity);
            }
            if (dPadDOWN_ButtonPressed) {
                MoveMouse(0, 1 * sensitivity);
            }
            if (dPadLEFT_ButtonPressed) {
                MoveMouse(-1 * sensitivity, 0);
            }
            if (dPadRIGHT_ButtonPressed) {
                MoveMouse(1 * sensitivity, 0);
            }

            // Check if A button is pressed & released
            if (isAButtonPressed) {
                if (!wasAButtonPressed) {
                    MouseClick(MOUSEEVENTF_LEFTDOWN);
                    wasAButtonPressed = true;
                }
            }
            else {
                if (wasAButtonPressed) {
                    MouseClick(MOUSEEVENTF_LEFTUP);
                    wasAButtonPressed = false;
                }
            }

            // Check if B button is pressed & released
            if (isBButtonPressed) {
                if (!wasBButtonPressed) {
                    MouseClick(MOUSEEVENTF_RIGHTDOWN);
                    wasBButtonPressed = true;
                }
            }
            else {
                if (wasBButtonPressed) {
                    MouseClick(MOUSEEVENTF_RIGHTUP);
                    wasBButtonPressed = false;
                }
            }

            // Check if LB button is pressed & released
            if (is_LB_ButtonPressed) {
                if (!wasLBButtonPressed) {
                    if (sensitivity > 1){
                        sensitivity--;
                    }
                    wasLBButtonPressed = true;
                }
            }
            else {
                if (wasLBButtonPressed) {
                    // released event
                    wasLBButtonPressed = false;
                }
            }

            // Check if RB button is pressed & released
            if (is_RB_ButtonPressed) {
                if (!wasRBButtonPressed) {
                    sensitivity++;
                    wasRBButtonPressed = true;
                }
            }
            else {
                if (wasRBButtonPressed) {
                    // released event
                    wasRBButtonPressed = false;
                }
            }
        }
        else {
            // Controller is not connected
            //std::cout << "Controller not connected" << std::endl;
        }

        Sleep(10); // Small delay to reduce CPU usage
    }

    return 0;
}
