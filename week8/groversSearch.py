import numpy as np

X_gate = np.array([[0, 1],   # Pauli X gate
                   [1, 0]])  # = NOT gate

H_gate = np.array([[1, 1],                      # Hadamard gate 
                   [1,-1]]) * np.sqrt(1/2)

Z_gate = np.array([[1, 0],                      # Pauli Z gate
                   [0,-1]])

TOFF_gate = np.array([[1, 0, 0, 0, 0, 0, 0, 0], # Toffoli gate
                      [0, 1, 0, 0, 0, 0, 0, 0],
                      [0, 0, 1, 0, 0, 0, 0, 0],
                      [0, 0, 0, 1, 0, 0, 0, 0],
                      [0, 0, 0, 0, 1, 0, 0, 0],
                      [0, 0, 0, 0, 0, 1, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 1],
                      [0, 0, 0, 0, 0, 0, 1, 0]])


CNOT_gate = np.array([
    [1,0,0,0],
    [0,1,0,0],
    [0,0,0,1],
    [0,0,1,0]
])


def probQubit(name):
    global workspace
    tosQubit(name)
    workspace = np.reshape(workspace,(-1,2))
    prob = np.linalg.norm(workspace,axis=0)**2
    return prob/prob.sum()                 # make sure sum is one

def measureQubit(name): 
    global workspace 
    global namestack
    prob = probQubit(name)
    measurement = np.random.choice(2,p=prob)
    workspace = (workspace[:,[measurement]]/
                 np.sqrt(prob[measurement]))
    namestack.pop()
    return str(measurement)


def tosQubit(name):
    global workspace
    global namestack
    k = len(namestack) - namestack.index(name)  # qubit pos
    if k > 1:
        namestack.append(namestack.pop(-k))  # rotate name stack
        workspace = np.reshape(workspace, (-1, 2, 2**(k-1)))
        workspace = np.swapaxes(workspace, -2, -1)


def pushQubit(name, weights):
    global workspace
    global namestack

    if workspace.shape == (1, 1):
        namestack = []

    namestack.append(name)

    weights = np.array(weights, dtype=workspace.dtype)
    weights = weights / np.linalg.norm(weights)

    workspace = np.reshape(workspace, (1, -1))
    workspace = np.kron(workspace, weights)


def applyGate(gate,*names):
    global workspace
    if list(names) != namestack[-len(names):]: # reorder stack
        for name in names: # if necessary 
            tosQubit(name)
    workspace = np.reshape(workspace,(-1,2**(len(names))))
    subworkspace = workspace[:,-gate.shape[0]:]
    np.matmul(subworkspace,gate.T,out=subworkspace)

def TOFFn_gate(ctl,result): # result = result XOR AND(qubits) 
    n = len(ctl)
    if n == 0: 
        applyGate(X_gate,result)
    if n == 1: 
        applyGate(CNOT_gate,ctl[0],result)
    elif n == 2: 
        applyGate(TOFF_gate,ctl[0],ctl[1],result)
    elif n > 2: 
        k=0
        while "temp"+str(k) in namestack: 
            k=k+1
        temp = "temp"+str(k)        # generate unique name 
        pushQubit(temp,[1,0])       # push zero temp qubit 
        applyGate(TOFF_gate,ctl[0],ctl[1],temp) # apply TOFF 
        ctl.append(temp)            # add temp to controls 
        TOFFn_gate(ctl[2:],result)  # recursion 
        applyGate(TOFF_gate,ctl[0],ctl[1],temp) # uncompute temp 
        measureQubit(temp)          # pop temp



def zero_booleanOracle(qubits, result):
    # if all qubits==0 return 1 else return 0

    for qubit in qubits:
        applyGate(X_gate, qubit)

    TOFFn_gate(qubits, result)

    for qubit in qubits:
        applyGate(X_gate, qubit)


def zero_phaseOracle(qubits):            # all qubits zero? 
    # if all qubits==0 return -weight else return weight
    for qubit in qubits:                 # negate all inputs
        applyGate(X_gate,qubit)
    applyGate(Z_gate,*namestack)         # controlled Z gate
    for qubit in qubits:                 # restore inputs
        applyGate(X_gate,qubit)


def sample_phaseOracle(qubits):          # sample function 
        # if all f(x)==1 return -weight else return weight
    applyGate(X_gate,qubits[1])          # negate qubit 1
    applyGate(Z_gate,*namestack)         # controlled Z gate
    applyGate(X_gate,qubits[1])          # restore qubit 1



def groverSearch(n, printProb=True):
    optimalTurns = int(np.pi/4*np.sqrt(2**n)-1/2)   # iterations 
    qubits = list(range(n))                         # generate qubit names
    for qubit in qubits:                            # initialize qubits
        pushQubit(qubit,[1,1])
    for k in range(optimalTurns):                   # Grover iterations:
        sample_phaseOracle(qubits)                  # apply phase oracle
        for qubit in qubits:                        # H-gate all qubits
            applyGate(H_gate,qubit) 
        zero_phaseOracle(qubits)                    # apply 0 phase oracle
        for qubit in qubits:                        # H-gate all qubits
            applyGate(H_gate,qubit) 
        if printProb:                               # peek probabilities
            print(probQubit(qubits[0]))             # to show convergence
    for qubit in reversed(qubits):                  # print result 
        print(measureQubit(qubit),end="")


workspace = np.array([[1.]])              # initialize workspace 
groverSearch(12)                           # (only reals used here)

