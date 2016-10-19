from pylab import *

N = 1000 # need this, need to change naming convention of files
# if name contains N, we could just fill arrays instead of lists
#filename = '../benchmarks/ES/positions_dt0.001000_N1000_vy6.280000.xyz'
filename = '../benchmarks/ESJ/pos_dt0.001000_N1000_m1.000000.xyz'
data = open(filename,'r')

# I am most definitely overcomplicating this
i = 0
j = 0
k = 0
m = 0
for line in data:
    if i == 0:
        num_bodies = int(line)
        positions = [zeros((N,3)) for a in range(num_bodies)]
        j += 1
    elif i == j*(num_bodies+2):
        k = 0
        j += 1
    elif not line.startswith('Comment'):
        a = line.split(' ')
        positions[k][int(m)] = (array([float(a[1]),float(a[2]),float(a[3])]))

        k += 1
        m += 1./num_bodies
    i += 1

# Plot ES

plot(positions[0][:,0],positions[0][:,1],label='Sun')
plot(positions[1][:,0],positions[1][:,1],label='Earth')
xlabel('x [AU]',size=17)
ylabel('y [AU]',size=17)
title('Earth-Sun system',size=17)
grid('on')
legend(fontsize=14)
savefig('../figures/earth_sun.png')
show()


# Plot ESJ

plot(positions[0][:,0],positions[0][:,1],label='Sun')
plot(positions[1][:,0],positions[1][:,1],label='Earth')
plot(positions[2][:,0],positions[2][:,1],label='Jupiter')
xlabel('x [AU]',size=17)
ylabel('y [AU]',size=17)
title('Earth-Sun-Jupiter system',size=17)
grid('on')
legend(fontsize=14)
savefig('../figures/earth_sun_jupiter.png')
show()
