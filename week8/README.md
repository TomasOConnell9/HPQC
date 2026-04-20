## Week 8 - Grover's Search – Part 2 

# Overview 
This week extends the concepts and functions developed in week 7 by implementing Grover's Search algorithm. The alorithm allows a quantum computer to search an unsorted database in O(\sqrt{$2^n$} time, making it much more efficent that classical search methods. The overall aim is to construct the necessary quantum function so that a Grover iteration loop can be implement to correctly relicate the solver.

# Grover's search

In a classical system, finding a specific item requires checking each state one by one until the correct state is found. Grover's search however uses quantum interference to increase the probability of the correct state.

The algorithm works by using a phase oracle to flip the sign of the correct state, a diffusion operation than reflects all amplitudes around their average values. Repeating this step multiple times will amplify the correct solution.

# Implementation
Next we will look at how the algorithm was implemented. The script can be found in the directory as `groversSearch.py`. Below will discuss the how the script works.

**Basics:**
The script reuses the same improved functions developed in Week 7. The `pushQubit`, `applyGate` and `measureQubit` are all vital for the algorithm to work correctly. The `pushQubit` functions works to construct the correct state vector. Equally, the `applyGate` function applies quantum gates to the system and finally the `measureQubit` function simulates the measurement of a qunatum state.

**Phase Oracles:**
The phase oracles seen in the script are key aspects of Grover's algorithm, as they identify the correct solution and marking it. First the `sample_phaseOracle` function works to identify the target state to the system by applying a phase shift to the correct input with a Z-gate.

The other phase oracle `zero_phaseOracle`, diffuses the |0...0> stae and applies a phase flip. The function temporarily converts all the 0 states into 1 so the function can perfrom operations on them. A Z-gate is the applied introducing a phase shift to the specific state. The X-gate then restores the system to its original basis. This function essentially dims the 'incorrect' values and increases the probability of the correct solution.

**The diffusion operator:**
The diffusion operators is responsible for the main amplification of the marked state. It works by applying a Hadamard gate to all the qubits, follows by the `zero_phaseOracle` function mentioned above, and then lastly a Hadamard gate again. Repeating this operation leads to a high probability of measuring the correction solution.

**Grover's iteration:**
The iteration loop found in `groverSearch` is the core of the algorithm. First the `groverSearch` function initialises all qubits to an equal superposition, which is done by pushing qubits with equal weights to essentially represent all possible states simultaneously. The loop then follows two mains steps. First the phase oracle marks the correct solution by flipping its sign. The diffusion operator then amplifies the probability of this state. With each iteration, the probability of the correct state increases. The number of iterations is given by np.pi / 4 * np.sqrt(2n) - 1 / 2**. After completing the iterations the qubits are measured and the result of the search is produced.


# Results
To test the perfromance of algorithim the probability distribution was recorded over each iteration. Each iteration shows the probability of the measured qubit being in either the 0 state or the 1 state which reflects how the system evolves over time.

**8 qubits:** the following table in the results of the script when ran for 8 qubits.

| Iteration | P(0)   | P(1)   |
| --------- | ------ | ------ |
| 1         | 0.4845 | 0.5155 |
| 2         | 0.4545 | 0.5455 |
| 3         | 0.4117 | 0.5883 |
| 4         | 0.3590 | 0.6410 |
| 5         | 0.2996 | 0.7004 |
| 6         | 0.2371 | 0.7629 |
| 7         | 0.1755 | 0.8245 |
| 8         | 0.1186 | 0.8814 |
| 9         | 0.0699 | 0.9301 |
| 10        | 0.0325 | 0.9675 |
| 11        | 0.0087 | 0.9913 |
| 12        | 0.0000 | 1.0000 |

These results demonstrate the process of amplitude amplification perfectly. In the first iteration the probability of this qubit being in state 0 or 1 is almost 50/50. As the iterations increases the value converges to a more "sure" result. Looking at iteration 6, the result is now almost 25/75 and then at the last iteration, 12, the result in sure the qubit is in the 1 state. This process is followed for the other 7 qubits and a final result is achieved.

The final output of the algorithm is 11111101, this represents the state with the highest probability of being the solution defined by the phase oracle. 

**12 qubits:**
| Iteration | P(0)    | P(1)   |
| --------- | ------- | ------ |
| 1         | 0.4990  | 0.5010 |
| 2         | 0.4971  | 0.5029 |
| 3         | 0.4942  | 0.5058 |
| 4         | 0.4903  | 0.5097 |
| 5         | 0.4855  | 0.5145 |
| 6         | 0.4798  | 0.5202 |
| 7         | 0.4731  | 0.5269 |
| 8         | 0.4657  | 0.5343 |
| 9         | 0.4573  | 0.5427 |
| 10        | 0.4482  | 0.5518 |
| 11        | 0.4383  | 0.5617 |
| 12        | 0.4276  | 0.5724 |
| 13        | 0.4163  | 0.5837 |
| 14        | 0.4043  | 0.5957 |
| 15        | 0.3917  | 0.6083 |
| 16        | 0.3785  | 0.6215 |
| 17        | 0.3649  | 0.6351 |
| 18        | 0.3508  | 0.6492 |
| 19        | 0.3363  | 0.6637 |
| 20        | 0.3214  | 0.6786 |
| 21        | 0.3063  | 0.6937 |
| 22        | 0.2910  | 0.7090 |
| 23        | 0.2755  | 0.7245 |
| 24        | 0.2599  | 0.7401 |
| 25        | 0.2443  | 0.7557 |
| 26        | 0.2287  | 0.7713 |
| 27        | 0.2132  | 0.7868 |
| 28        | 0.1978  | 0.8022 |
| 29        | 0.1826  | 0.8174 |
| 30        | 0.1677  | 0.8323 |
| 31        | 0.1531  | 0.8469 |
| 32        | 0.1389  | 0.8611 |
| 33        | 0.1252  | 0.8748 |
| 34        | 0.1119  | 0.8881 |
| 35        | 0.0991  | 0.9009 |
| 36        | 0.0870  | 0.9130 |
| 37        | 0.0754  | 0.9246 |
| 38        | 0.0646  | 0.9354 |
| 39        | 0.0545  | 0.9455 |
| 40        | 0.0451  | 0.9549 |
| 41        | 0.0366  | 0.9634 |
| 42        | 0.0289  | 0.9711 |
| 43        | 0.0220  | 0.9780 |
| 44        | 0.0160  | 0.9840 |
| 45        | 0.0110  | 0.9890 |
| 46        | 0.0069  | 0.9931 |
| 47        | 0.0037  | 0.9963 |
| 48        | 0.0015  | 0.9985 |
| 49        | 0.00028 | 0.9997 |

By increasing the size to 12, the amount of iterations grows. This is the expected relationship as the dimentions increase by 2^n, where n is the number of qubits. The results shown here highlight how for larger qubits the increased iterations is needed. In the 8 bit example we discussed how by iteration 6 the system was 75% sure of the correct state, now in this 12 bit state the algorithm is still roughly 50/50 by iteration 6. 


#Conclusion
This weeks task successfully implemented Grover's algorithm, using the concepts developed in last weeks task. The implementation demonstates how quantum phase oracles and diffusion operators can be combined to search for a correct solution in an unordered dataset. The results clearly showed the convergence towards the correct solution, with the probability of the target state increasing over the iterations. 
