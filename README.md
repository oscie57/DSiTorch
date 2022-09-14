# DSiTorch

DSi Homebrew Application for enabling the torch on the back of the system.

Currently, only the devKitPro's NDS example is set up.

Special thanks to [Haifisch](https://github.com/haifisch) for helping me out!

`camera.c` and other camera stuff from [dsi-camera](https://github.com/Epicpkmn11/dsi-camera)

## Concept

How the application will hopefully work is have information on the top screen, with the bottom screen having a button that will toggle the torch, with it being white and having black text saying "On" when on and black with white text saying "Off". When you press A or the touch screen, make a sound play.

## Evie — Today at 01:05

For the light you don't actually need to turn on the camera, just [aptina.c#L142](https://github.com/oscie57/DSiTorch/blob/main/arm7/source/aptina.c#L142) to enable and [aptina.c#L153](https://github.com/oscie57/DSiTorch/blob/main/arm7/source/aptina.c#L153) to disable... I see I forgot to comment that as the comments are all just from GBATEK and that wasn't from GBATEK

[//]: <> ( C:\devkitPro\tools\bin )
