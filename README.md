# Orange Pi 6 Plus experiments
Experiments with Orange Pi 6 Plus (CIX P1 SBC) and MIPI-CSI camera (ov13855).

## Supported cameras

Soc | Board | Distro | Memory size | Kernel version | Camera | sensor | Max. window size | fps
|:--|:--|:--|:--|:--|:--|:--|:--|:--|
| CIX P1 | Orange Pi 6 Plus | Debian 12 | 32 GB | 6.1.44-cix  | camera1 | ov13855 | 1920x1080 | 30 fps
| CIX P1 | Orange Pi 6 Plus | Debian 12 | 32 GB | 6.1.44-cix  | camera2 | ov13855 | 1920x1080 | 30 fps

Table of Contents:

- [Introduction](#introduction)
- [MIPI Camera OV13855](#mipi-camera-ov13855)
- [Gstreamer](#gstreamer)
- [Gstreamer - Display camera content on screen](#gstreamer---display-camera-content-on-screen)
  - [Camera 1](#camera-1)
  - [Camera 2](#camera-2)
  - [Dual camera](#dual-camera)
- [GStreamer hardware encoder and decoder](#gstreamer-hardware-encoder-and-decoder)
  - [Gstreamer encoder](#gstreamer-encoder)
    - [Gstreamer pipeline encoder H265](#gstreamer-pipeline-encoder-h265)
  - [Gstreamer decoder](#gstreamer-decoder)
  	- [Gstreamer pipeline decoder H265](#gstreamer-pipeline-decoder-h265)
- [Record video and display it on the screen](#record-video-and-display-it-on-the-screen)
- [Real-time streaming](#real-time-streaming)
  - [Localhost live streaming](#localhost-live-streaming)
  - [Network live streaming](#network-live-streaming)
  - [RTSP live streaming](#rtsp-live-streaming)
- [NPU](#npu)
  - [Camera with NPU](#camera-with-npu)
- [Issues](#issues)
- [Acknowledgments](#acknowledgments)

## Introduction

For the experiments i will use an Orange Pi 6 Plus, 32 GB of ram, and dual camera with ov13855 sensor and Debian 12 with GNOME (wayland).

I'll conduct some experiments with the camera1 and camera2 and the NPU as soon as cix-opencv deb package is available for the Orange Pi 6 Plus (CIX P1 SBC).

The kernel version for these experiments is kernel 6.1-cix ("stock" version) and, if i'm brave enough i will build and rerun the experiments with kernel 6.6.

The board is considered, at the moment of these experiments a developer version, so not ready for an everyday use if you consider stability a premise.

The reason is the v4l2 commands are not truly v4l2 compliant and any attempt to use it can cause the kernel to crash or require a full reboot.

That being said, gstreamer comes to the rescue, but an incorrect pipeline can also cause a reboot.

I would rather use Weston, which is faster than GNOME, but i could not run a Weston session or switch to any TTY and run Weston from there, if someone was able to do this, please advise.

Contents will be added/changed soon. This is a WiP...

A simple v4l2 command will reboot the board:

```
v4l2-ctl -d /dev/video1 --all
Driver Info:
	Driver name      : arm-china-isp
	Card type        : linlon isp v4l2
	Bus info         : platform:armcb_isp_v4l2-00
	Driver version   : 6.1.44
	Capabilities     : 0x85201000
		Video Capture Multiplanar
		Read/Write
		Streaming
		Extended Pix Format
		Device Capabilities
	Device Caps      : 0x05201000
		Video Capture Multiplanar
		Read/Write
		Streaming
		Extended Pix Format
Media Driver Info:
	Driver name      : imgsensor0
	Model            : armcb_isp_v4l2
	Serial           : 
	Bus info         : platform:armcb_isp_v4l2-000
	Media version    : 6.1.44
	Hardware revision: 0x00000000 (0)
	Driver version   : 6.1.44
Interface Info:
	ID               : 0x03000002
	Type             : V4L Video
Entity Info:
	ID               : 0x00000001 (1)
	Name             : armcb-00-vid-cap
	Function         : V4L2 I/O
Priority: 2
```


## MIPI Camera OV13855

Orange Pi 6 Plus has two MIPI-CSI camera interfaces, cam1 and cam2 and can be used at the same time.

The ov13855 sensor is capable of 4224x3136@30fps but for some reason i was able to grab only 1920x1080@30fps, 
maybe an updated firmware or if the source code is released we can improve things here.

The cameras are attached to cam1 and cam2 connectors near the sd card slot, see the manual for reference.
In order to use cam1 and cam2 at the same time you must change the isp service that manage the cameras.

**1. Change the line:**
```
ExecStart=/usr/bin/isp_app -s 0 &
to
ExecStart=/usr/bin/isp_app -m 2 &
```

**2. Use mcedit to edit, change the line and Save it with F2 then quit with F10**

```
sudo mcedit /lib/systemd/system/isp-daemon.service
[Unit]
Description=ISP Daemon
After=network.target load-isp-modules.service

[Service]
Type=simple
Environment=LD_LIBRARY_PATH="/usr/share/cix/lib"
ExecStart=/usr/bin/isp_app -m 2 &
Restart=always
RestartSec=1
StartLimitInterval=10
StartLimitBurst=5

[Install]
WantedBy=multi-user.target
```

**3. Reboot the board with:**

```
sudo reboot
```
## Gstreamer

Gstreamer is a poweful tool to help grab the frames and display it on screen.
It can also utilize the Hardware encoder to record video from the camera(s) or stream videos in real-time across the network.
At the other end, you can utilize the Hardware decoder to display the stream in real-time or display the recorded file.
Currently only gstreamer can be used.

Gstreamer is provided by CIX/RADXA team.

### Gstreamer - Display camera content on screen

#### Camera 1

![H265 Camera1_fps_1920x1080 streaming](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/camera1.png)
![H265 Cam1 1920x1080 streaming](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/cam1.png)
![H265 Cam1 1920x1080 streaming](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/cam1-cpu-load.png)

- Pipeline:
	```
	gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoconvert ! glimagesink
 	```

#### Camera 2

![H265 Cam2 1920x1080 streaming](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/camera2.png)

- Pipeline:
	```
	gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoconvert ! fakesink v4l2src device=/dev/video3 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoconvert ! glimagesink
 	```

#### Dual camera

![H265 Dual_Camera 1920x1080 streaming](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/dual-cam.png)

- Pipeline:

 	**Type the command in the Terminal and move the first image on the screen to the side or resize the window with your mouse.**
	```
	gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoconvert ! autovideosink v4l2src device=/dev/video3 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoconvert ! autovideosink
 	```

   
## GStreamer hardware encoder and decoder

Hardware encoding and decoding are performed by the VPU (Video Processing Unit), which accelerates video encoding and decoding, making video playback more power-efficient and freeing up the CPU for other tasks.

There are several encoders available with Gstreamer, but we will focus on Hardware Encoder: H265 (HEVC).

```
gst-inspect-1.0 | grep 'video4' | grep 'Encoder'
video4linux2:  v4l2h264enc: V4L2 H.264 Encoder
video4linux2:  v4l2h265enc: V4L2 H.265 Encoder
video4linux2:  v4l2jpegenc: V4L2 JPEG Encoder
video4linux2:  v4l2vp8enc: V4L2 VP8 Encoder
video4linux2:  v4l2vp9enc: V4L2 VP9 Encoder
```

There are several decoders available with Gstreamer, but we will focus on Hardware Decoder: H265 (HEVC).

```
gst-inspect-1.0 | grep 'video4' | grep 'Decoder'
video4linux2:  v4l2av1dec: V4L2 AV1 Decoder
video4linux2:  v4l2h263dec: V4L2 H263 Decoder
video4linux2:  v4l2h264dec: V4L2 H264 Decoder
video4linux2:  v4l2h265dec: V4L2 H265 Decoder
video4linux2:  v4l2jpegdec: V4L2 JPEG Decoder
video4linux2:  v4l2mpeg2dec: V4L2 MPEG2 Decoder
video4linux2:  v4l2mpeg4dec: V4L2 MPEG4 Decoder
video4linux2:  v4l2vp8dec: V4L2 VP8 Decoder
video4linux2:  v4l2vp9dec: V4L2 VP9 Decoder
```

Enable VPU monitoring before running the experiments

```
echo 3 | sudo tee /sys/kernel/debug/amvx/log/group/perf/enable > /dev/null
```


### Gstreamer encoder

For this experiments the Hardware encoder **v4l2h265enc** will be used.


#### Gstreamer pipeline encoder H265

To record video with **H.265** (**HEVC**) encoder and Matroska (MKV) container, use the following pipelines:

- Camera1

  This pipeline will save the video to the file **video_camera1_1920x1080.mkv**

	
		
		gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoparse width=1920 height=1080 framerate=30/1 format=nv12 ! video/x-raw,colorimetry=bt709 ! v4l2h265enc capture-io-mode=mmap output-io-mode=dmabuf extra-controls="encode,fixed_qp=28" ! video/x-h265,profile=main,level=\(string\)5 ! h265parse ! matroskamux ! filesink location=video_camera1_1920x1080.mkv
		


- Camera2

  This pipeline will save the video to the file **video_camera2_1920x1080.mkv**
  
		
		gst-launch-1.0 v4l2src device=/dev/video3 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoparse width=1920 height=1080 framerate=30/1 format=nv12 ! video/x-raw,colorimetry=bt709 ! v4l2h265enc capture-io-mode=mmap output-io-mode=dmabuf extra-controls="encode,fixed_qp=28" ! video/x-h265,profile=main,level=\(string\)5 ! h265parse ! matroskamux ! filesink location=video_camera2_1920x1080.mkv
		



- Camera1 and Camera2 at the same time

  This pipeline will save the video to the files **video_camera1_1920x1080.mkv** and  **video_camera1_1920x1080.mkv** at the same time
  
		
		gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoparse width=1920 height=1080 framerate=30/1 format=nv12 ! video/x-raw,colorimetry=bt709 ! v4l2h265enc capture-io-mode=mmap output-io-mode=dmabuf extra-controls="encode,fixed_qp=28" ! video/x-h265,profile=main,level=\(string\)5 ! h265parse ! matroskamux ! filesink location=video_camera1_1920x1080.mkv v4l2src device=/dev/video3 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoparse width=1920 height=1080 framerate=30/1 format=nv12 ! video/x-raw,colorimetry=bt709 ! v4l2h265enc capture-io-mode=mmap output-io-mode=dmabuf extra-controls="encode,fixed_qp=28" ! video/x-h265,profile=main,level=\(string\)5 ! h265parse ! matroskamux ! filesink location=video_camera2_1920x1080.mkv
		



### Gstreamer decoder

For this experiments the Hardware decoder **v4l2h265dec** will be used.

#### Gstreamer pipeline decoder H265

To display the **H.265** (**HEVC**) video recorded above in **MKV**, the following pipelines can be used:
	
  - using **decodebin** for any encoder
	
		gst-launch-1.0 filesrc location=video_camera1_1920x1080.mkv ! decodebin ! glupload ! glcolorconvert ! glcolorbalance ! glimagesink

- using **v4l2h265dec** for MKV container

		gst-launch-1.0 filesrc location=video_camera1_1920x1080.mkv ! matroskademux ! h265parse ! v4l2h265dec ! fpsdisplaysink video-sink=autovideosink text-overlay=true


## Record video and display it on the screen

To record video and display it on screen at the same time, use the following pipeline:

  	gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,format=NV12, width=1920, height=1080 ! tee name=t t. ! queue ! videoparse width=1920 height=1080 framerate=30/1 format=nv12 ! video/x-raw,colorimetry=bt709 ! v4l2h265enc capture-io-mode=mmap output-io-mode=dmabuf extra-controls="encode,fixed_qp=28" ! video/x-h265,profile=main,level=\(string\)5 ! filesink location=video_camera1_1920x1080.hevc t. ! queue ! videoconvert ! glimagesink


## Real-time streaming


This experiment streams videos from the Orange Pi 6 Plus to an Intel Box, the encoder used is H.265 (HEVC) and decoded and displayed on the receiving end (Intel Box),
Live streaming with RTSP experiments.


### Localhost live streaming

[Client: Orangepi 6 plus] --> [H265 encoder] --> [TCP - localhost] --> [Server: Orangepi 6 plus] --> [H265 decoder] --> [Display it on screen]

**Client**

  	gst-launch-1.0 tcpserversrc port=5000 host=0.0.0.0 ! h265parse ! v4l2h265dec ! fpsdisplaysink video-sink=autovideosink text-overlay=true sync=false

**Server**

	gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoparse width=1920 height=1080 framerate=30/1 format=nv12 ! video/x-raw,colorimetry=bt709 ! v4l2h265enc capture-io-mode=mmap output-io-mode=dmabuf extra-controls="encode,fixed_qp=28" ! video/x-h265,profile=main,level=\(string\)5 ! tcpclientsink host=localhost port=5000

### Network live streaming

[Client: Orangepi 6 plus] --> [H265 encoder] --> [TCP - ethernet] --> [Server: Intel Box] --> [H265 decoder] --> [Display it on screen]

![H265 Dual Cam 1920x1080 streaming](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/dual-cam.png)
![H265 Dual Cam 1920x1080 streaming](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/dual-cam-cpu.png)

**Client (Orange Pi 6 Plus)**

IP: 192.168.254.75

  	gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,format=NV12, width=1920, height=1080 ! videoparse width=1920 height=1080 framerate=30/1 format=nv12 ! video/x-raw,colorimetry=bt709 ! v4l2h265enc capture-io-mode=mmap output-io-mode=dmabuf extra-controls="encode,fixed_qp=28" ! video/x-h265,profile=main,level=\(string\)5 ! tcpclientsink host=192.168.254.253 port=5000

**Server (Intel Box)**

IP: 192.168.254.253

  	gst-launch-1.0 tcpserversrc port=5000 host=0.0.0.0 ! h265parse ! v4l2h265dec ! fpsdisplaysink video-sink=autovideosink text-overlay=true sync=false

```
cat /sys/kernel/debug/amvx/log/group/perf/utilization
VPU Utilization: 29.83%

cat /sys/kernel/debug/amvx/log/group/perf/realtime_fps 
13:39:44 ~ 13:47:16 [ffff0002886d4138] HEVC encoder 1920x1080 12979 frames, current fps 29.92, average fps 28.27
13:39:45 ~ 13:47:16 [ffff0002886c4138] HEVC encoder 1920x1080 12477 frames, current fps 26.16, average fps 27.23
```


### RTSP live streaming

to be completed.


## NPU

The NPU experiments with the camera will be possible when cix-opencv deb package is available, it might have some Hardware acceleration.
With the help of v4l2loopback i'm able to run python3 yolox demo v4l2 from the SDK.

The trick is to redirect the camera output to an v4l2looback device, in the example below we can see NPU inference with live streaming using a Webcam 720p and mipi-csi ov13855 (1920x1080).
Webcam does not require the use of v4l2loopback.

**NPU with webcam**
![Webcam load](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/yolox_demo_webcam_load.png)

**NPU with MIPI-CSI camera (ov13855)**

![MIPI-CSI 1920x1080](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/yolox_demo_mipi_camera.png)

![MIPI-CSI 1920x1080 load](https://raw.githubusercontent.com/avafinger/orangepi-6-plus-experiments/refs/heads/main/yolox_demo_mpi_camera_load.png)

to be completed.


## Issues

- Once you issue a wrong command, the kernel may crash, or a reboot is needed.
- Camera2 seems to be in preview mode
- Camera2 has some tearings, most likely a DMA buf issue
- Cameras are capped at 1920x1080
- When streaming both Camera1 and Camera2 at the same time, the latency is noticeable (My Intel Box is ancient)

## Acknowledgments

I would like to thank me and myself for taking the time
