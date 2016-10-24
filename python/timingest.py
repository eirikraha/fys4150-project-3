from pylab import *
from scipy.optimize import curve_fit

filename = "../benchmarks/ES/timing.txt"
data = open(filename, 'r')

counter = 0
V_counter = 0
E_counter = 0

N_list = []
Verlet_time = []
Euler_time = []

for line in data:
    word = line.strip(' ')
    word = word.split(',')
    temp_split = word[0].split(' ')
    if word[0] == 'Verlet':
        a = word[2].strip()
        a = a.split(' ')
        N_list.append(float(a[-1]))
        V_counter += 1
    elif temp_split[0] == 't' and V_counter == 1:
        a = temp_split[-1].strip()
        Verlet_time.append(float(a))
        V_counter = 0
    elif word[0] == 'Euler':
        E_counter += 1
    elif temp_split[0] == 't' and E_counter == 1:
        a = temp_split[-1].strip()
        Euler_time.append(float(a))
        E_counter = 0
    counter += 1

V_arr = array(Verlet_time)
E_arr = array(Euler_time)
N_arr = array(N_list)
#print log(N_arr)
#print log(V_arr)
V_eq = polyfit(log10(N_arr), log10(V_arr), 2)
E_eq = polyfit(log10(N_arr), log10(E_arr), 2)
#print V_eq
#print E_eq

#def func(x, a, b, c):
#    return a*exp(-b*x) + c

x1 = linspace(0, log10(N_arr[-1]), 1000)
V_y = [polyval(V_eq, i) for i in x1]
E_y = [polyval(E_eq, i) for i in x1]

#ppot, pcov = curve_fit(func, N_arr, V_arr)
#print ppot

#V_yexp = func(x1, ppot[0], ppot[1], ppot[2])

plot(log10(N_arr), log10(V_arr), '*b', label='Verlet Velocity')
plot(log10(N_arr), log10(E_arr), '*y', label='Forward Euler')
plot(x1, V_y, '-b', label = r'%1.2ex^2 + %1.2ex + %1.2e' %(V_eq[0], V_eq[1], V_eq[2]))
plot(x1, E_y, '-y', label = r'%1.2ex^2 + %1.2ex + %1.2e' %(E_eq[0], E_eq[1], E_eq[2]))
xlabel('Number of timesteps')
ylabel('Time used [s]')
legend(loc=2)
savefig("../figures/timeusage.png")
#xscale('log')
#yscale('log')
show()

plot(linspace(0, len(N_arr), len(N_arr)), V_arr/E_arr)
show()
