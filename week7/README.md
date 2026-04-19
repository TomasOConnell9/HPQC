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

These results demonstrate how each of the gates transforms the state vectors to new vectors. The X gate performs a flipping of states, the Y gate also flips the state but introduces a complex phase shift, resulting in an output with imaginary components instead of real. The Z gate leaves the state unchanged in this example, and finally the Hadamard gate (H) creates a suerposition from the definite state [[1. 0.]], giving an equal combination of both staes.

# Part 2:

The next part of the process involved adding a new function called tosQubit. This operations rearranges the quantum state so that the selected quibit is moved to the top of the stock (tos). This is done as gate operations are applied to the leading qubits in a state, so we need a function that can move a given qubit to the top of the stack.

The function uses swapaxes from the NumPy library to rearrange the structure of the quantum state. After reshaping it so that the qubit is isolated, swapaxes exchanges the location of the desired qubit with the top position in the stack. This fuction moves qubits into their desired location without changing the quantum information contained in the state. 

To investigate how this works, the `tos.py` script was developed to implement and test the tosQubit fucntion.

The script first pushes a [1,0] qubit and then follows pushing a second qubit [0.6,0.8], which forms a two-qubit state, rusing in the combined state vector: [[0.6 0.8 0.  0. ]]. When the tosQubit(2) function is applied to this state vector the swapaxes fucntion turns the new state vector into [[0.6 0. 0.8 0.]] as expected. To observe this, run the following line,

- 'python3 tos.py'

# Part3: 
The final core part of this weeks simulation was turning these state vectors into physical measurements implementing a `probQubit` and `measureQubit` function. 

First, the `probQubit` function calculates the probability of the top qubit in the stack being either a 1 or a 0. This is done by calculating the square norm of each value.

Next, the `measureQubit` function uses the probabilities to selcted a specific outcome for each state using `np.random.choice`. This is to highlight the probabilistic nature of quantum mechanics. 

To verify these functions run the `measuring_qubits.py` function running,

- 'python3 measuring_qubits.py`

The script has a qubit initialised with weights [0.6,0.8], these wights correspond to probabilities of 0.36 for 0 and 0.64 for 1. When the user runs the script they are prompted to enter how many measurements they would like to run, as the number of measurements increases the outcomes should converge to this distribution.

**Result**

| Number of measurements | Outcome                    |
|----------------------- | -------------------------- |
| 1                      | up: 0.0%, down: 1.0%       |
| 10                     | up: 0.5%, down: 0.5%       |
| 1000                   | up: 0.31%, down: 0.69%     |
| 10000                  | up: 0.3574%, down: 0.6426% |

As the number of measurements increases, the observed outcome converges towards the expected probabilities for the quantum state.

