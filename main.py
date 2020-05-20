import MC_LIB
from mpi4py import MPI
import sys
import os
import glob
import time
import numpy as np

comm = MPI.COMM_WORLD
my_rank = comm.Get_rank()
p = comm.Get_size()

#simulation parameters
nr_of_atoms = 100
box_len = 125.
max_stepsize = 2.

endpoint = 250000 #Amount of attempted steps before simulation ends

#Function that returs standard deviation of list of values
def stdev(inputlist):
    if len(inputlist) < 2:
        raise ValueError('standard deviation requires at least two data points')
    avg = (sum(inputlist))/len(inputlist)
    variance = sum((x-avg)**2 for x in inputlist)
    variance = variance/(len(inputlist) -1)
    return variance**0.5


if my_rank != 0:
    msg = ""
    seed = my_rank
    mc = MC_LIB.Monte_carlo(nr_of_atoms, box_len, seed)
    amount_of_rejects = 0
    amount_of_accepts = 0
    t_start = time.process_time()
    while((amount_of_rejects+amount_of_accepts)<endpoint):
        if(mc.try_move(max_stepsize)):
            amount_of_accepts +=1
        else:
            amount_of_rejects += 1

    t_stop = time.process_time()
    msg += "Total time ran: "
    msg += str(t_stop - t_start)
    msg += " seconds"
    msg += "\n"
    msg += "Amount of attempted moves made in simulation: "
    msg += str(amount_of_accepts+amount_of_rejects)
    msg += "\n"
    msg += "Amount of accepted moves: "
    msg += str(amount_of_accepts)
    msg += "\n"
    msg += "Acceptance rate: "
    msg += str(amount_of_accepts/(amount_of_accepts+amount_of_rejects))
    msg += "\n"
    msg += "final energy: "
    msg += str(mc.total_energy())
    msg += "\n"
    msg += "----------------------------------------- \n\n"
    comm.send(msg, dest=0, tag=1)   #send msg to print to log to main thread
    comm.send(mc.total_energy(), dest=0, tag=2) #send energy to add to energy_list
else:
    energies = []
    #comparison of numpy rand vs own rand:
    print("10 random doubles between 0 and 1 generated via NUMPY: ")
    for i in range(10):
        print(np.random.rand())
    print()
    print("10 random doubles between 0 and 1 generated via OWN RNG: ")
    for seed in range(1,11):
        print(MC_LIB.gen_rng(seed))
    ###################################################################

    #start monte carlo simulation
    for procid in range(1,p):
        message = comm.recv(source = procid, tag=1)
        energies.append(comm.recv(source = procid, tag=2))
        strproc = str(procid)
        f = open(strproc + ".tmp", "w+")
        f.write("PROCESS " + strproc + ": \n" + message)
        f.close()
    read_files = glob.glob("*.tmp")
    with open("results.log", "wb+") as outfile:
        for i in range(1,p):
            with open("%d.tmp" % i, "rb") as infile:
                outfile.write(infile.read())
            os.remove("%d.tmp" % i)


    with open("results.log", "a+") as outfile:
        outfile.write("\n")
        outfile.write("Average energy: %f \n" % ((sum(energies))/len(energies)) )
        outfile.write("Standard deviation of sample: %f \n" % (stdev(energies)) )
        outfile.write("Lowest energy from set: %f" % (min(energies)))
    print("Sim finished")
    print("Results printed to results.log")
