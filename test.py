import os
import subprocess
import time
import matplotlib.pyplot as plt

num_t = [1, 2, 3, 4]
for i in range(5, 505, 5):
    num_t.append(i)

for i in num_t:
    os.system("{ time ./main -t " + str(i) + " ./sample_input/largeCase_in.txt ./out1.txt ; } 2>> time.txt")

real = []
user = []
sys = []

for _ in range(24):
    d, a, b, c = input(), input(), input(), input()
    real.append(float(a.split('m')[1].split('s')[0]))
    user.append(float(b.split('m')[1].split('s')[0]))
    sys.append(float(c.split('m')[1].split('s')[0]))

plt.plot(num_t, real, label="real time")
plt.plot(num_t, user, label="user time")
plt.plot(num_t, sys, label="system cpu time")
plt.legend()
plt.xlabel("numbers of thread")
plt.ylabel("times")

plt.savefig("run_time_test")
