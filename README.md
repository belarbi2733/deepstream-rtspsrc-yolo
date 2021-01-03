*****************************************************************************
* Copyright (c) 2019-2020 NVIDIA Corporation.  All rights reserved.
*
* NVIDIA Corporation and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA Corporation is strictly prohibited.
*****************************************************************************

# Deepstream rtspsrc YOLO

The program detect objects from RTSP source and create RTSP output.
It is made from Deepstream sample apps.
YOLOv4 pretrained model is trained using [https://github.com/AlexeyAB/darknet](https://github.com/AlexeyAB/darknet)`.
Pre-trained model detects 10 classes of shells.
When RTSP source is disconnected, the APP wait for restarting RTSP source and try reset pipeline.

## Demo



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

This YOLO model detect 10 classes.

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

### Jetson

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
