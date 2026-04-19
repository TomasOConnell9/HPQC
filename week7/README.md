## Week 7 - Program your own quantum computer – Part 1 

# Overview 
This week the first step of simulating a qunatum computer in Python was built. The processes of building the simulator involves several stages, this week focuses on representing the quantum state in Python, how qubits are constructed and added to systems as well as introducing the basic quantum gates and manipulation of the quantum states. 

# Part 1: 
The first part of the simulator involved devloping two functions, `pushQubit` and `applyGate`. These functions can be found in the `pushing_qubits_onto_stack.py` script in the week 7 repository. `pushQubit` takes a qubit and pushes it to a current quantum state by applying a Kroneckere product between the exisiting state and the new qubit. This results in the qubit becoming part of the system and the state vector dubling in size to account for all possible combinations of the new qubit. Next, the `applyGate` function performs operations on the qunatum state by applying predefined quantum gates on the state. This in done with matrix multiplication allowing for the flipping of qubits or the development of new superpositions.

To investigate how these gates work I developed the `gate_operations.py` script. This scripts acts as a simple testing enviroment so users can select a quantum gate and observe the effect that it has on a basic qunatum state. 

- To run the script users srun 'python3 gate_operations.py'.

Once ran users will be promted to select which gate they want to apply to the state. This will then print the original quantum state followed by the state after the operations is performed, this will allow the user to investigate how the gate affects the state vector. 

**Results**

To demonstate how the script works here are four qunatum gates acting on a standard qubit. Each test begings with the input [[1. 0.]] and the output corresponds to the result of the matrix multiplication induced by the resepctive gate. 

| Input     | Gate    | Output                     |
| --------- | ------  | -------------------------- |
| [[1. 0.]] |  X-Gate | [[0. 1.]]                  |
| [[1. 0.]] |  Y-Gate | [[0.+0.j 0.+1.j]]          |
| [[1. 0.]] |  Z-Gate | [[1. 0.]]                  |
| [[1. 0.]] |  H-Gate | [[0.70710678 0.70710678]]  |

