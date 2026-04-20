## Week 8 - Grover's Search – Part 2 

# Overview 
This week extends the concepts and functions developed in week 7 by implementing Grover's Search algorithm. The alorithm allows a quantum computer to search an unsorted database in O(\sqrt{$2^n$} time, making it much more efficent that classical search methods. The overall aim is to construct the necessary quantum function so that a Grover iteration loop can be implement to correctly relicate the solver.

# Grover's search

In a classical system, finding a specific item requires checking each state one by one until the correct state is found. Grover's search however uses quantum interference to increase the probability of the correct state.

The algorithm works by using a phase oracle to flip the sign of the correct state, a diffusion operation than reflects all amplitudes around their average values. Repeating this step multiple times will amplify the correct solution.

# Implementation
Next we will look at how the algorithm was implemented. The script can be found in the directory as `groversSearch.py`. Below will discuss the how the script works.

**Basics**
The script reuses the same improved functions developed in Week 7. The `pushQubit`, `applyGate` and `measureQubit` are all vital for the algorithm to work correctly. The `pushQubit` functions works to construct the correct state vector. Equally, the `applyGate` function applies quantum gates to the system and finally the `measureQubit` function simulates the measurement of a qunatum state.

**Phase Oracles**
The phase oracles seen in the script are key aspects of Grover's algorithm, as they identify the correct solution and marking it. First the `sample_phaseOracle` function works to identify the target state to the system by applying a phase shift to the correct input with a Z-gate.

The other phase oracle `zero_phaseOracle`, diffuses the |0...0> stae and applies a phase flip. The function temporarily converts all the 0 states into 1 so the function can perfrom operations on them. A Z-gate is the applied introducing a phase shift to the specific state. The X-gate then restores the system to its original basis. This function essentially dims the 'incorrect' values and increases the probability of the correct solution.

**The diffusion operator**
The diffusion operators is responsible for the main amplification of the marked state. It works by applying a Hadamard gate to all the qubits, follows by the `zero_phaseOracle` function mentioned above, and then lastly a Hadamard gate again. Repeating this operation leads to a high probability of measuring the correction solution.

**Grover's iteration**
The iteration loop found in `groverSearch` is the core of the algorithm. First the `groverSearch` function initialises all qubits to an equal superposition, which is done by pushing qubits with equal weights to essentially represent all possible states simultaneously. The loop then follows two mains steps. First the phase oracle marks the correct solution by flipping its sign. The diffusion operator then amplifies the probability of this state. With each iteration, the probability of the correct state increases. The number of iterations required is proportional to the square root of the number of possible states. After completing the iterations the qubits are measured and the result of the search is produced.
