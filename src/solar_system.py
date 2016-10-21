from pylab import *
import sys

def read_file(filename, N):
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
        
    return positions


if __name__=='__main__':
    try:
        task = sys.argv[1]
    except IndexError:
        task = 'ESJ'

    N = 1000 # need this, need to change naming convention of files
    # if name contains N, we could just fill arrays instead of lists

    # Plot ES
    if task=='ES':
        filename = '../benchmarks/ES/pos_dt0.001000_N1000_vy6.280000.xyz'
        positions = read_file(filename,N)

        plot(positions[0][:,0],positions[0][:,1],label='Sun')
        plot(positions[1][:,0],positions[1][:,1],label='Earth')
        xlabel('x [AU]',size=17)
        ylabel('y [AU]',size=17)
        title('Earth-Sun system',size=17)
        grid('on')
        legend(fontsize=14)
        xlim([-1.1,1.1])
        ylim([-1.1,1.1])
        savefig('../figures/earth_sun.png')
        show()


    # Plot ESJ
    if task=='ESJ':
        dt = '0.001000'
        m = 1
        filename = '../benchmarks/ESJ/pos_dt%s_N%d_m%.6f.xyz'%(dt,N,m)
        positions = read_file(filename,N)

        plot(positions[0][0,0],positions[0][0,1],'oy',ms=10,label='Sun')
        plot(positions[1][0,0],positions[1][0,1],'ob',ms=10,label='Earth')
        plot(positions[2][0,0],positions[2][0,1],'og',ms=10,label='Jupiter')
        
        plot(positions[0][:,0],positions[0][:,1],'y')
        plot(positions[1][:,0],positions[1][:,1],'b')
        plot(positions[2][:,0],positions[2][:,1],'g')
        xlabel('x [AU]',size=17)
        ylabel('y [AU]',size=17)
        title('Earth-Sun-Jupiter system',size=17)
        grid('on')
        legend(numpoints=1,fontsize=14)
        savefig('../figures/earth_sun_jupiter.png')
        show()

            # Plot ES
    if task=='GM':
        filename = '../benchmarks/GM/pos_dt0.010000_N1000.xyz'
        positions = read_file(filename,N)

        plot(positions[0][:,0],positions[0][:,1],label='Sun')
        plot(positions[1][:,0],positions[1][:,1],label='Mercury')
        xlabel('x [AU]',size=17)
        ylabel('y [AU]',size=17)
        title('Mercury-Sun system',size=17)
        grid('on')
        legend(fontsize=14)
        #xlim([-1.1e1,1.1e1])
        #ylim([-1.1e3,1.1e3])
        savefig('../figures/Mercury_sun.png')
        show()
