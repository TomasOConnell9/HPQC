#simple script to test different gates, takes user input (what gate they want to see) and gives a before and after reading.

import numpy as np

def pushQubit(weights):
    global workspace
    workspace = np.reshape(workspace,(1,-1))
    workspace = np.kron(workspace,weights)

def applyGate(gate):
    global workspace
    workspace = np.reshape(workspace,(-1,gate.shape[0]))
    np.matmul(workspace,gate.T,out=workspace)


#all the possible gates given by the tutorial
X_gate = np.array([[0, 1],
                   [1, 0]])

Y_gate = np.array([[ 0,-1j],
                   [1j,  0]])

Z_gate = np.array([[1, 0],
                   [0,-1]])

H_gate = np.array([[1, 1],
                   [1,-1]]) * np.sqrt(1/2)

S_gate = np.array([[1, 0],
                   [0,1j]])

T_gate = np.array([[1,                0],
                   [0,np.exp(np.pi/-4j)]])

Tinv_gate = np.array([[1, 0],
                      [0,np.exp(np.pi/4j)]])

def P_gate(phi):
    return np.array([[1,             0],
                     [0,np.exp(phi*1j)]])

def Rx_gate(theta):
    return np.array([[np.cos(theta/2),   -1j*np.sin(theta/2)],
                     [-1j*np.sin(theta/2), np.cos(theta/2)]])

def Ry_gate(theta):
    return np.array([[np.cos(theta/2), -np.sin(theta/2)],
                     [np.sin(theta/2),  np.cos(theta/2)]])

def Rz_gate(theta):
    return np.array([[np.exp(-1j*theta/2),             0],
                     [                  0, np.exp(1j*theta/2)]])

CNOT_gate = np.array([[1, 0, 0, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1],
                      [0, 0, 1, 0]])

CZ_gate = np.array([[1, 0, 0, 0],
                    [0, 1, 0, 0],
                    [0, 0, 1, 0],
                    [0, 0, 0,-1]])

SWAP_gate = np.array([[1, 0, 0, 0],
                      [0, 0, 1, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1]])

TOFF_gate = np.array([[1,0,0,0,0,0,0,0],
                      [0,1,0,0,0,0,0,0],
                      [0,0,1,0,0,0,0,0],
                      [0,0,0,1,0,0,0,0],
                      [0,0,0,0,1,0,0,0],
                      [0,0,0,0,0,1,0,0],
                      [0,0,0,0,0,0,0,1],
                      [0,0,0,0,0,0,1,0]])

# this function runs whatever gate the user selects
def run_gate(choice):
    global workspace

    if choice == "1":
        workspace = np.array([[1.]])
        pushQubit([1, 0])
        print("before:", workspace)
        applyGate(X_gate)
        print("after: ", workspace)

    elif choice == "2":
        workspace = np.array([[1.+0j]])
        pushQubit([1, 0])
        print("before:", workspace)
        applyGate(Y_gate)
        print("after: ", workspace)

    elif choice == "3":
        workspace = np.array([[1.]])
        pushQubit([1, 0])
        print("before:", workspace)
        applyGate(Z_gate)
        print("after: ", workspace)

    elif choice == "4":
        workspace = np.array([[1.]])
        pushQubit([1, 0])
        print("before:", workspace)
        applyGate(H_gate)
        print("after: ", workspace)

    elif choice == "5":
        workspace = np.array([[1.+0j]])
        pushQubit([1, 0])
        print("before:", workspace)
        applyGate(S_gate)
        print("after: ", workspace)

    elif choice == "6":
        workspace = np.array([[1.+0j]])
        pushQubit([1, 0])
        applyGate(H_gate)
        print("before:", workspace)
        applyGate(T_gate)
        print("after: ", workspace)

    elif choice == "7":
        workspace = np.array([[1.+0j]])
        pushQubit([1, 0])
        applyGate(H_gate)
        print("before:", workspace)
        applyGate(Tinv_gate)
        print("after: ", workspace)

    elif choice == "8":
        phi = float(input("Enter phi (radians): "))
        workspace = np.array([[1.+0j]])
        pushQubit([1, 0])
        applyGate(H_gate)
        print("before:", workspace)
        applyGate(P_gate(phi))
        print("after: ", workspace)

    elif choice == "9":
        theta = float(input("Enter theta (radians): "))
        workspace = np.array([[1.+0j]])
        pushQubit([1, 0])
        print("before:", workspace)
        applyGate(Rx_gate(theta))
        print("after: ", workspace)

    elif choice == "10":
        theta = float(input("Enter theta (radians): "))
        workspace = np.array([[1.]])
        pushQubit([1, 0])
        print("before:", workspace)
        applyGate(Ry_gate(theta))
        print("after: ", workspace)

    elif choice == "11":
        theta = float(input("Enter theta (radians): "))
        workspace = np.array([[1.+0j]])
        pushQubit([1, 0])
        print("before:", workspace)
        applyGate(Rz_gate(theta))
        print("after: ", workspace)

    elif choice == "12":
        workspace = np.array([[1.]])
        pushQubit([1, 0])
        pushQubit([0.6, 0.8])
        print("before:", workspace)
        applyGate(CNOT_gate)
        print("after: ", workspace)

    elif choice == "13":
        workspace = np.array([[1.]])
        pushQubit([1, 0])
        pushQubit([0.6, 0.8])
        print("before:", workspace)
        applyGate(CZ_gate)
        print("after: ", workspace)

    elif choice == "14":
        workspace = np.array([[1.]])
        pushQubit([1, 0])
        pushQubit([0.6, 0.8])
        print("before:", workspace)
        applyGate(SWAP_gate)
        print("after: ", workspace)

    elif choice == "15":
        workspace = np.array([[1.]])
        pushQubit([1, 0])
        pushQubit([0.6, 0.8])
        pushQubit([1, 0])
        print("before:", workspace)
        applyGate(TOFF_gate)
        print("after: ", workspace)

    else:
        print("Invalid choice")


print("Which gate do you want to test?")
print(" 1  - X gate")
print(" 2  - Y gate")
print(" 3  - Z gate")
print(" 4  - H gate")
print(" 5  - S gate")
print(" 6  - T gate")
print(" 7  - Tinv gate")
print(" 8  - P gate")
print(" 9  - Rx gate")
print(" 10 - Ry gate")
print(" 11 - Rz gate")
print(" 12 - CNOT gate")
print(" 13 - CZ gate")
print(" 14 - SWAP gate")
print(" 15 - TOFF gate")

choice = input("Enter number: ").strip()
run_gate(choice)
