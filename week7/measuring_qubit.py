import numpy as np

def pushQubit(weights):
	global workspace
	workspace = np.reshape(workspace, (1,-1))
	workspace = np.kron(workspace, weights)

def probQubit():
	global workspace
	workspace = np.reshape(workspace, (-1, 2))
	return np.linalg.norm(workspace, axis=0)**2

def measureQubit():
	global workspace
	prob = probQubit()
	measurement = np.random.choice(2, p=prob)
	workspace = (workspace[:, [measurement]] / np.sqrt(prob[measurement]))
	return str(measurement)

workspace = np.array([[1. ]])

for n in range(30):
        pushQubit([0.6,0.8])
        result = measureQubit()
        #print(measureQubit(), end="\n")


#small test to show that see if measureQubit's results will converge to the square amplitudes of the pushed qubit
up = 0
down = 0
total_measurements = int(input("How many measurements?"))

for n in range(total_measurements):
	pushQubit([0.6,0.8])
	result = measureQubit()

	if result == "0":
		up += 1
	else:
		down += 1

print(f"up: {up}, down: {down}")
up_percent = up / total_measurements
down_percent = down / total_measurements
print(f"up: {up_percent}, down: {down_percent}")
