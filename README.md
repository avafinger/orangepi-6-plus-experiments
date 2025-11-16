# Orange Pi 6 Plus experiments
These are my experiments with the cameras on CIX P1 SBC available on Orange Pi 6 Plus

## Supported cameras

Soc | Board | Distro | Memory size | Kernel version | Camera | sensor | Max. window size | fps
|:--|:--|:--|:--|:--|:--|:--|:--|:--|
| CIX P1 | Orange Pi 6 Plus | Debian 12 | 32 GB | 6.1.44-cix  | camera1 | ov13855 | 1920x1080 | 30 fps
| CIX P1 | Orange Pi 6 Plus | Debian 12 | 32 GB | 6.1.44-cix  | camera2 | ov13855 | 1920x1080 | 30 fps

Table of Contents:

- [Introduction](#introduction)
- [Gstreamer](#gstreamer)
- [MIPI Camera OV13855](#MIPI Camera OV13855)
- [Gstreamer](#gstreamer)
  - [H264](#h264)
    - [Gstreamer pipeline H264](#gstreamer-pipeline-h264)
  - [H265](#h265)
    - [Gstreamer pipeline H265](#gstreamer-pipeline-h265)
- [Hardware decoder](#hardware-decoder)
  - [Gstreamer decoder](#gstreamer-decoder)
- [Hardware encoder](#hardware-encoder)
- [Real time streaming](#real-time-streamer)
- [NPU](#npu)
  - [SDL2](#sdl2)
  - [SDL3](#sdl3)
- [Issues](#issues)
- [Acknowledgments](#acknowledgments)

## Introduction

For the experiments i will use an Orange Pi 6 Plus, 32 GB of ram, and dual camera with ov13855 sensor and Debian 12 with GNOME (wayland).
I'll conduct some experiments with the camera1 and camera2 and the NPU as soon as cix-opencv deb package is available for the Orange Pi 6 Plus (CIX P1 SBC).
The kernel version for these experiments is kernel 6.1-cix ("stock" version) and if i'm brave enough i will build and rerun the experiments with kernel 6.6.
The board is considered, at the moment of these experiments a developer version, so not ready for an everyday use if you consider stability a premise.
The reason is the v4l2 commands are not truly v4l2 compliant and any attempt to use it can cause the kernel to crash or require a full reboot.
That being said, gstreamer comes to the rescue, but an incorrect pipeline can also cause a reboot.

I would rather use Weston, which is faster than GNOME, but i could not run a Weston session or switch to any TTY and run Weston from there, if someone was able to do this, please advise.

Contents will be added/changed soon. This is a WiP...

## Gstreamer

Gstreamer is a powefull tool to help grab the frames and display it on screen.
It can also utilize the Hardware encoder to record video from the camera(s) or stream videos in real-time across the network.
At the other end, you can utilize the Hardware decoder to display the stream in real-time or display the recorded file.
Currently only gstreamer can be used.

## MIPI Camera OV13855

The ov13855 sensor is capable of 4224x3136@30fps but for some reason i was able to grab only 1920x1080@30fps, 
maybe an updated firmware or if the source code is released we can improve things here.

to be completed.

## Hardware decoder

to be completed.

## Hardware encoder

to be completed.

## Real time streamer

## NPU

The NPU experiments with the camera will be possible when cix-opencv deb package is available, it might have some Hardware acceleration

to be completed.

## SDL2

to be completed.

## SDL3

to be completed.

## Issues

Once you issue a wrong command, the kernel may crash, or a reboot is needed.

## Acknowledgments

I would like to thank me and myself for taking the time
