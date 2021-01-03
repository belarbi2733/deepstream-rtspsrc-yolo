# Deepstream rtspsrc YOLO

<img src="https://raw.githubusercontent.com/ryokubozono/deepstream-rtspsrc-yolo/main/system_drawing.png" alt="system_drawing" title="system_drawing">

This app detect objects from RTSP source and create RTSP output.

This app is made from Deepstream sample-apps.

YOLOv4 pre-trained model is trained using [https://github.com/AlexeyAB/darknet](https://github.com/AlexeyAB/darknet).

ONNX model is created using [https://github.com/Tianxiaomo/pytorch-YOLOv4](https://github.com/Tianxiaomo/pytorch-YOLOv4).

Pre-trained model detects 10 classes of shells.

When RTSP source is disconnected, the APP wait for restarting RTSP source and try reset pipeline.

## Demo

[![demo](http://img.youtube.com/vi/t9wc6LU26wg/0.jpg)](http://www.youtube.com/watch?v=t9wc6LU26wg "demo")

## Environment

### Jetson

- higher model than Jetson Nano Developer Kit(2GB)
- Jetpack 4.4.1
- CUDA 10.2
- TensorRT 7.1.3.0
- 64GB microSD card

## Set up Jetson 

Doc for Jetson Nano: [https://developer.nvidia.com/embedded/learn/get-started-jetson-nano-devkit](https://developer.nvidia.com/embedded/learn/get-started-jetson-nano-devkit)

## YOLO

### Build custom YOLOv4

```
cd yolo/nvdsinfer_custom_impl_Yolo
make
```

You will find `yolo/nvdsinfer_custom_impl_Yolo/libnvdsinfer_custom_impl_Yolo.so` and use it as pgie's `custom-lib-path`.

### Download pre-trained ONNX model

This YOLOv4 model detects 10 classes of shell.

Google Drive: [https://drive.google.com/file/d/1MDd2u1Rn9xemOjd4fjJhm8SIHGqoieF8/view?usp=sharing](https://drive.google.com/file/d/1MDd2u1Rn9xemOjd4fjJhm8SIHGqoieF8/view?usp=sharing)

### Convert ONNX model to TensorRT model

Set path to `trtexec`.

```
export PATH=/usr/local/cuda-10.2/bin:/usr/src/tensorrt/bin:$PATH
```

Convert `<onnx_file_name>` to `<engine_file_name>`.

```
cd yolo
trtexec \
    --onnx=<onnx_file_name> \
    --explicitBatch \
    --saveEngine=<engine_file_name> \
    --minShapes=input:1x3x416x416 \
    --optShapes=input:1x3x416x416 \
    --maxShapes=input:1x3x416x416 \
    --shapes=input:1x3x416x416 \
    --fp16
```

## Run prediction

Compile app.

```
export CUDA_VER=10.2
make
```

Run app with `<rtsp_source_uri>`.

```
./deepstream-rtspsrc-yolo-app \
    <rtsp_source_uri>
```

Rum app with multi streams.

```
./deepstream-rtspsrc-yolo-app \
    <rtsp_source_uri_1> \
    <rtsp_source_uri_2>
```

### Watch predicted streams.

`rtsp://<Jestson's IP>:8554/dt-test`

### Others

#### PGIE's interval

Update interval in `config/pgie_config.txt` to decrease Jetson's work load.

This config means skip frames of RTSP source every `interval` number.

```
interval=5
```

#### Custom YOLO model

If you want to use custom YOLOv4 or YOLOv4-tiny model, you need the following tasks.

- train model and get weight file.
- convert weight file to onnx file.
- convert onnx file to tensorRT file.

#### Custom classes number

If you want to change pgie classes number form 10, you neet to change 

- `NUM_CLASSES_YOLO` in `yolo/nvdsinfer_custom_impl_Yolo/nvdsparsebbox_Yolo.cpp`
- `PGIE_DETECTED_CLASS_NUM` and `pgie_classes_str` in `src/deepstream_rtspsrc_yolo.cpp`
- `num-detected-classes` in `config/pgie_config.txt`

*****************************************************************************
* Copyright (c) 2019-2020 NVIDIA Corporation.  All rights reserved.
*
* NVIDIA Corporation and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA Corporation is strictly prohibited.
*****************************************************************************
