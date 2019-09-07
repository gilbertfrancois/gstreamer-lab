#!/bin/bash

echo "-- Installing gstreamer packages..."

$ sudo apt install \
    cmake \
    awscli \
    libssl1.0.0 \
    libgstreamer1.0-0 \
    gstreamer1.0-tools \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav \
    libgstrtspserver-1.0-0 \
    libjansson4=2.11-1

echo "-- Downloading test data"

aws --profile ava-x-skuld s3 sync --sse AES256 s3://apothecary.smbh.ch/gst-lab/models/ models/
aws --profile ava-x-skuld s3 sync --sse AES256 s3://apothecary.smbh.ch/gst-lab/3rdparty/ 3rdparty/
