import numpy as np

def pushQubit(weights):
    global workspace
    workspace = np.reshape(workspace,(1,-1))
    workspace = np.kron(workspace,weights)

def tosQubit(k):
	global workspace
	if k > 1:                                               # if non-trivial
		workspace = np.reshape(workspace,(-1,2,2**(k-1)))
		workspace = np.swapaxes(workspace,-2,-1)


workspace = np.array([[1.]])
pushQubit([1,0])
pushQubit([0.6,0.8])
#print(workspace)
tosQubit(2)
#print(workspace)
print(np.reshape(workspace,(1,-1))) #if you want to reshape after tosQubit
