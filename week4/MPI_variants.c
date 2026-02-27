#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

void check_uni_size(int uni_size);
void root_task(int uni_size);
void client_task(int my_rank, int uni_size);
void perform_task(int my_rank, int uni_size);


int main(int argc, char **argv)
{
        int ierror = 0;

        int my_rank = 0;
        int uni_size = 0;

        ierror = MPI_Init(&argc, &argv);

        ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

        check_uni_size(uni_size);
        perform_task(my_rank, uni_size);


        MPI_Finalize();

        return 0;
}

//ensure there are enough processes
void check_uni_size(int uni_size)
{
        if (uni_size >= 2)
        {
                return; //two or more processes continue the program
        }
        else //less than two aport
        {
                fprintf(stderr, "Unable to communicate with fewer than 1 procesess. Please enter 2 or more procesess");
                exit(-1);
        }
}

//task to perform the root process
void root_task(int uni_size)
{
        int recv_message = 0; //intitalise buffer for incomong message
        int count = 1;
        int source;
        int tag = 0;

        MPI_Status status;

        for (source = 1; source < uni_size; source++)
        {
                MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status); ///recieve messages from client processes

                printf("Hello, I am 0 of %d. Received %d from Rank %d\n", uni_size, recv_message, source);
        }
}

//task to perform for all non root processes
void client_task(int my_rank, int uni_size)
{
        int dest;
        dest = 0;
        int send_message;
        send_message = my_rank * 10;
	double start_time;
	double end_time;
	start_time =MPI_Wtime();

	//Five variants of MPI_Send()

	//1. MPI_Send, default blocking send, MPI decided if to use buffering or synchronous
        MPI_Send(&send_message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

	//2. MPI_Ssend, requires acknowledgement from reciever before sending
	//MPI_Ssend(&send_message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

	//3. MPI_Bsend, message is copied into a user defined buffer. Send returns straight away and is unaware if root actually recieved the information
	//int buffer_attached_size = MPI_BSEND_OVERHEAD + sizeof(int); //need a user defined buffer, this calculates the size needed
	//char *buffer = malloc(buffer_attached_size); //allocate the buffer
	//MPI_Buffer_attach(buffer, buffer_attached_size); //attach the buffer to MPI so it can be used for the task
	//MPI_Bsend(&send_message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD); //perform the buffer send
	//MPI_Buffer_detach(&buffer, &buffer_attached_size); //detach the buffer once empty
	//free(buffer); //free the buffer memory

	//4. MPI_Rsend(), assumes root has already started MPI_Recv()
	//MPI_Rsend(&send_message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

	//5. MPI_Isend(), non blocking send that returns immediatley
	//MPI_Request request;
	//MPI_Status status;
	//MPI_Isend(&send_message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, &request);
	//MPI_Wait(&request, &status);//ensure completion before continuing

	end_time = MPI_Wtime();
        printf("Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
	printf("Rank %d took %.10f seconds to send\n", my_rank, end_time - start_time);//test to see how long it takes to send and recieve all the messages
}


void perform_task(int my_rank, int uni_size)
{
        if (my_rank == 0)
        {
                root_task(uni_size);
        }
        else
        {
                client_task(my_rank, uni_size);
        }
}

