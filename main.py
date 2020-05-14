import example

nr_of_atoms = 500
box_len = 50.
max_stepsize = 2.

mc = example.Monte_carlo(nr_of_atoms, box_len)
while(True):
    accepted = mc.try_move(max_stepsize)
