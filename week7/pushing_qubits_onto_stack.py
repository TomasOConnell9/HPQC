import numpy as np

def pushQubit(weights):
    global workspace
    workspace = np.reshape(workspace,(1,-1))
    workspace = np.kron(workspace,weights)


workspace = np.array([[1.]])
pushQubit([1,0])
#print(workspace)
pushQubit([3/5,4/5])
#print(workspace)


def applyGate(gate):
    global workspace
    workspace = np.reshape(workspace,(-1,gate.shape[0]))
    np.matmul(workspace,gate.T,out=workspace)

X_gate = np.array([[0, 1],
                   [1, 0]])

H_gate = np.array([[1, 1],
                  [1,-1]]) * np.sqrt(1/2)

T_gate = np.array([[1,0],
                   [0,np.exp(np.pi/-4j)]])

SWAP_gate = np.array([[1, 0, 0, 0],
                      [0, 0, 1, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1]])

#np.matmul(X_gate,[1,0])

workspace = np.array([[1. +0j]])
pushQubit([1,0])
#print("input",workspace)
#applyGate(X_gate)
applyGate(H_gate)
applyGate(T_gate)
#print("output",workspace)

#swapping qbits
workspace = np.array([[1.]])
pushQubit([1,0])
pushQubit([0.6,0.8])
print(workspace)
applyGate(SWAP_gate)
print(workspace)

