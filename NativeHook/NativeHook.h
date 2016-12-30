//==============================================================================
//               NativeHook.h
//
// describe : Based on win32 API, blocking keyboard input.
//            Prevent user from switching fullscreen to desktop by Win, Alt+F4
//            Alt+Tab, Ctrl+Esc, you can add your own blocking rules.
//==============================================================================

#ifndef NATIVEHOOK_H
#define NATIVEHOOK_H

//==============================================================================
//
//                 CNativeHook
//
//==============================================================================

class CNativeHook
{
public:
    CNativeHook();
    ~CNativeHook();

private:
    // set hook, enable blocking keyboard feature
    void SetHook();
    // unset hook, disable blocking keyboard feature
    void CancelHook();
};

#endif  // NATIVEHOOK_H

