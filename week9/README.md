# Week 9- GPU Quantum Computing Simulation

## Overview
Week 9 investigates the final step of simulating a quantum computer in Python. The simulator now attempts to move the calculations from the CPU to the GPU using the PyTouch library. The calcaultions performed in the simulator are ideal for a GPU due to the parallel nature.


## Part 1

The first part of this week is to install and import the PyTorch package. From there users can ensure that their system has a compatible GPU to use. If no GPU is detected the script will inform the user a CPU is avalible.
To check users can use the `GPU_check.py` script by running `python3 GPU_check.py`
