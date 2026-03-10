import numpy as np

def pushQubit(weights):
    global workspace
    workspace = np.reshape(workspace,(1,-1))
    workspace = np.kron(workspace,weights)


workspace = np.array([[1.]])
pushQubit([1,0])
print(workspace)
pushQubit([3/5,4/5])
print(workspace)
