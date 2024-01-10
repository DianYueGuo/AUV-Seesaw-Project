#!/bin/bash

IMAGE_NAME=ros_bridge
docker build -f Dockerfile -t "${IMAGE_NAME}" .
