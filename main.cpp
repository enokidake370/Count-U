#include <stdbool.h>
#include <string.h>

#include <coreinit/time.h>
#include <vpad/input.h>
#include <whb/log_console.h>
#include <whb/log.h>
#include <whb/proc.h>

// Random variables for the game.
int buttonPressCount = 0;
int activeColorIndex = 0;
bool isTouchScreenHeld = false;

void updateButtonPressCount()
{
    buttonPressCount += 1;
    if (buttonPressCount == 1)
    {
        // The very first button is pressed, grammar matters.
        WHBLogPrintf("Total %d Count!", buttonPressCount);
    }
    else
    {
        // Anything that isn't the first button press.
        WHBLogPrintf("Total %d Count!", buttonPressCount);
    }
    if (buttonPressCount % 100 == 0)
    {
        // Small vibration every hundred presses.
        uint8_t pattern[15];
        memset(pattern, 0x99, 15);
        VPADControlMotor(VPAD_CHAN_0, pattern, 60);
        // Change the color from red, to blue, etc.
        if (activeColorIndex == 0)
        {
            WHBLogConsoleSetColor(0x99000000);
            activeColorIndex = 1;
        }
        else if (activeColorIndex == 1)
        {
            WHBLogConsoleSetColor(0x00990000);
            activeColorIndex = 2;
        }
        else if (activeColorIndex == 2)
        {
            WHBLogConsoleSetColor(0x00009900);
            activeColorIndex = 3;
        }
        else if (activeColorIndex == 3)
        {
            WHBLogConsoleSetColor(0x99000000);
            activeColorIndex = 1;
        }
    }
}

int main()
{
    // Initialize everything needed for the game.
    WHBProcInit();
    WHBLogConsoleInit();
    // Set some variables for the Wii U GamePad.
    VPADStatus input;
    VPADReadError error;
    // Print the console header.
    WHBLogConsoleSetColor(0x000000);
    WHBLogPrint("Count U");
    WHBLogPrint("by Yuta!");
    WHBLogPrint("---------------");
    WHBLogConsoleDraw();

    while (WHBProcIsRunning())
    {
        // Watch the Wii U GamePad for button presses.
        VPADRead(VPAD_CHAN_0, &input, 1, &error);

        if (input.tpNormal.touched != 0)
        {
            // Check if the screen is pressed on.
            if (isTouchScreenHeld == false)
            {
                // Change the color from red, to blue, etc.
                isTouchScreenHeld = true;
                if (activeColorIndex == 0)
                {
                    WHBLogConsoleSetColor(0x99000000);
                    activeColorIndex = 1;
                }
                else if (activeColorIndex == 1)
                {
                    WHBLogConsoleSetColor(0x00990000);
                    activeColorIndex = 2;
                }
                else if (activeColorIndex == 2)
                {
                    WHBLogConsoleSetColor(0x00009900);
                    activeColorIndex = 3;
                }
                else if (activeColorIndex == 3)
                {
                    WHBLogConsoleSetColor(0x00000000);
                    activeColorIndex = 0;
                }
            }
        }
        else
        {
            isTouchScreenHeld = false;
        }

        if (input.trigger & VPAD_BUTTON_A)
        {
            updateButtonPressCount();
        }
        WHBLogConsoleDraw();
    }
    // Clear everything when the game is exited.
    WHBLogConsoleFree();
    WHBProcShutdown();
    return 0;
}