from pylab import *

filename1 = "../benchmarks/GR/ang_perVerlet_dt1.0e-07_years100.000000"
filename2 = "../benchmarks/GR/ang_perVerletREL_dt1.0e-07_years100.000000"

data1 = open(filename1, 'r')
data2 = open(filename2, 'r')

counter = 0
for line1 in data1:
    counter += 1

verletarr = zeros(counter)
verletrelarr = zeros(counter)

counter2 = 0
for line1 in data1:
    verletarr[counter2] = line1.split(' ')[3]
    counter2 += 1

counter3 = 0
for line2 in data2:
    #print counter3
    verletrelarr[counter3] = line2.split(' ')[3]
    counter3 += 1

perhelioncheck = verletarr - verletrelarr
#for i in perhelioncheck:
    #print "%1.2e" %(i)

x = linspace(0, counter, counter)

plot(x, perhelioncheck)
title('dt = 1e-7', fontsize = '18')
ylabel('Radians difference', fontsize = '16')
xlabel('Revolutions around the Sun', fontsize = '16')
ticklabel_format(style = 'sci', axis = 'y', scilimits = (0,0))
savefig('../figures/perheliondt1e-7.png')
show()
    #print "%1.2e" %(float(Verlet[3]) - float(VerletREL[3]))
        
        
