from pylab import *
import sys, glob

def read_file(filename, N):
    data = open(filename,'r')
    totlines = 0
    for line in data:
        totlines += 1
    data.close()
    data = open(filename,'r')
    # I am most definitely overcomplicating this
    i = 0
    j = 0
    k = 0
    m = 0
    for line in data:
        if i == 0:
            num_bodies = int(line)
            positions = [zeros((totlines/(num_bodies+2),3)) for a in range(num_bodies)]
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
    data.close()
    return positions


if __name__=='__main__':
    try:
        task = sys.argv[1]
    except IndexError:
        task = 'ES'

    N = 2000 # need this, need to change naming convention of files
    # if name contains N, we could just fill arrays instead of lists

    # Plot ES
    if task=='ES':
        files = glob.glob('../benchmarks/ES/pos*.xyz')
        for filename in files:
            words = filename.split('_')
            method = words[1]
            dt = float((words[2])[2::])
            years = int((words[3])[3::])
            vy = float((words[4])[2:-4])

            # Plotting
            positions = read_file(filename,1)

            plot(positions[0][0,0],positions[0][0,1],'oy',ms=20,label='Sun')
            plot(positions[1][0,0],positions[1][0,1],'ob',ms=10,label='Earth')
            
            plot(positions[0][:,0],positions[0][:,1],'y',lw=2.0)
            plot(positions[1][:,0],positions[1][:,1],'b',lw=2.0)
            xlabel('x [AU]',size=17)
            ylabel('y [AU]',size=17)
            legend(numpoints=1,fontsize=14,loc='best')
            grid('on')
            if vy == 6.28:
                title('Earth-Sun system, %s, dt=%.1e'%(method,dt),size=17)
                xlim([-1.2,1.2])
                ylim([-1.2,1.2])
                savefig('../figures/%s/ES_%s_dt%.1e_vy%.2f.png'%(task,method,dt,vy))

            else:
                title('Earth-Sun system, %s, dt=%.1e, v=%.2f'%(method,dt,vy),size=17)
                xlim([-2,2])
                ylim([-2,3])
                savefig('../figures/%s/ES_%s_dt%.1e_yrs%d_vy%.2f.png'%(task,method,dt,years,vy))
            show()


    # Plot ESJ
    if task=='ESJ1' or task=='ESJ2':
        files = glob.glob('../benchmarks/%s/pos*.xyz'%task)
        for filename in files:
            words = filename.split('_')
            dt = float((words[1])[2::])            
            #N = int((words[2])[1::])
            m = float((words[3])[1:-4])
        
            # Plotting
            positions = read_file(filename,N)
            
            plot(positions[0][0,0],positions[0][0,1],'oy',ms=10,label='Sun')
            plot(positions[1][0,0],positions[1][0,1],'ob',ms=10,label='Earth')
            plot(positions[2][0,0],positions[2][0,1],'og',ms=10,label='Jupiter')
        
            plot(positions[0][:,0],positions[0][:,1],'y',lw=2.0)
            plot(positions[1][:,0],positions[1][:,1],'b',lw=2.0)
            plot(positions[2][:,0],positions[2][:,1],'g',lw=2.0)
            xlabel('x [AU]',size=17)
            ylabel('y [AU]',size=17)
            
            if task=='ESJ2':
                title(r'Earth-Sun-Jupiter, dt=%.1e'%(dt),size=17)
            else:
                title(r'Earth-Sun-Jupiter, dt=%.1e, m=%.1f$\cdot M_J$'%(dt,m),size=17)

            grid('on')
            legend(numpoints=1,fontsize=14)
            xlim([-6.5,6.5])
            ylim([-6.5,6.5])
            savefig('../figures/%s/%s_dt%.1e_m%.1f.png'%(task,task,dt,m))
            show()


    # Plot WS
    if task=='WS':
        bodies = ['Sun','Mercury','Venus','Earth','Mars','Jupiter',
                  'Saturn','Uranus','Neptune','Pluto']
        colors = ['y','k','w','b','r','m','g','c','0.3','0.8']
        files = glob.glob('../benchmarks/WS/pos*.xyz')
        for filename in files:
            words = filename.split('_')
            dt = float((words[1])[2::])
            years = int(float(((words[2])[3:-4])))
        
            # Plotting
            positions = read_file(filename,N)
            
            start = 0
            end = 6
            for i in range(start,len(positions)- end):
                plot(positions[i][:,0],positions[i][:,1],color=colors[i],lw=2.0,label=bodies[i])
            xlabel('x [AU]',size=17)
            ylabel('y [AU]',size=17)            

            title(r'Full solar system, dt=%.1e, %d years'%(dt,years),size=17)
            grid('on')
            legend(numpoints=1,fontsize=14)
#            xlim([-6.5,6.5])
#            ylim([-6.5,6.5])
            savefig('../figures/WS/pos%d%d_dt%.1e_yrs%.1f.png'%(start,end,dt,years))
            show()


    # Plot GR
    if task=='GR':
        filename = '../benchmarks/GR/pos_dt0.010000_N1000.xyz'
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
