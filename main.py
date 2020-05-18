import MC_LIB
from mpi4py import MPI
import sys
import os
import glob
import time

comm = MPI.COMM_WORLD
my_rank = comm.Get_rank()
p = comm.Get_size()

nr_of_atoms = 500
box_len = 50.
max_stepsize = 2.

endpoint = 25 #Amount of rejects in a row before simulation is stopped

if my_rank != 0:
    msg = ""
    seed = my_rank
    mc = MC_LIB.Monte_carlo(nr_of_atoms, box_len, seed)
    amount_of_rejects = 0
    amount_of_accepts = 0
    t_start = time.process_time()
    while(amount_of_rejects <endpoint):
        if(mc.try_move(max_stepsize)):
            amount_of_rejects = 0
            amount_of_accepts +=1
        else:
            amount_of_rejects += 1
        #print("moves: ", amount_of_moves, "energy: ", mc.total_energy())
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
    comm.send(msg, dest=0)
else:
    for procid in range(1,p):
        message = comm.recv(source = procid)
        strproc = str(procid)
        f = open(strproc + ".tmp", "w+")
        f.write("PROCESS " + strproc + ": \n" + message)
        print("Simulation of proc: ", procid, "ended: ", message)
        f.close()
    read_files = glob.glob("*.tmp")
    with open("results.log", "wb+") as outfile:
        for i in range(1,p):
            with open("%d.tmp" % i, "rb") as infile:
                outfile.write(infile.read())
            os.remove("%d.tmp" % i)
    print("sim finished")


    print(MC_LIB.gen_rng(5))


#print("simulation finished")
"""
mc = MC_LIB.Monte_carlo(nr_of_atoms, box_len)
amount_of_rejects = 0 #Amount of rejected moves in a row. Used to determine end of run
while(amount_of_rejects <50):
    if(mc.try_move(max_stepsize)):
        amount_of_rejects = 0
    else:
        amount_of_rejects += 1
"""
