from pylab import *
from solar_system import *
from mpl_toolkits.mplot3d import Axes3D

# Makes 3D plots of the desired systems
bodies = ['Sun','Mercury','Venus','Earth','Mars','Jupiter',
           'Saturn','Uranus','Neptune','Pluto']
colors = ['y','k','w','b','r','m','g','c','0.3','0.8']

task = 'WS'
N = 1000

if task=='ESJ':
    dt = '0.010000'
    m = 1
    filename = '../benchmarks/ESJ/pos_dt%s_N%d_m%.6f.xyz'%(dt,N,m)
    pos = read_file(filename,N)
    
    fig3d = figure()
    ax3d = fig3d.add_subplot(111,projection='3d')
    ax3d.plot(pos[0][0:2,0],pos[0][0:2,1],pos[0][0:2,2],'oy',ms=10,label='Sun')
    ax3d.plot(pos[1][0:2,0],pos[1][0:2,1],pos[1][0:2,2],'ob',ms=10,label='Earth')
    ax3d.plot(pos[2][0:2,0],pos[2][0:2,1],pos[2][0:2,2],'og',ms=10,label='Jupiter')
    
    ax3d.plot(pos[0][:,0],pos[0][:,1],pos[0][:,2],'y')
    ax3d.plot(pos[1][:,0],pos[1][:,1],pos[1][:,2],'b')
    ax3d.plot(pos[2][:,0],pos[2][:,1],pos[2][:,2],'g')
    ax3d.set_xlabel('x [AU]',size=15)
    ax3d.set_ylabel('y [AU]',size=15)
    ax3d.set_title(r'Earth-Sun-Jupiter system in 3D, %d$\cdot M_J$'%m,size=15)
    #grid('on')
    ax3d.legend(numpoints=1,fontsize=14)
    fig3d.savefig('../figures/earth_sun_jupiter.png')
    show()
        

if task=='WS':
    dt = '0.010000'
    m = 1
    filename = '../benchmarks/WS/pos_dt%s_N%d.xyz'%(dt,N)
    pos = read_file(filename,N)
    
    fig3d = figure()
    ax3d = fig3d.add_subplot(111,projection='3d')

    for i in range(len(pos)-3):
#        ax3d.plot(pos[i][0],pos[i][0],'o',color=colors[i],ms=10,label=bodies[i])
    
        ax3d.plot(pos[i][:,0],pos[i][:,1],pos[i][:,2],color=colors[i])

    ax3d.set_xlabel('x [AU]',size=15)
    ax3d.set_ylabel('y [AU]',size=15)
    ax3d.set_title(r'Full solar system in 3D',size=15)
    ax3d.set_zlim(-5,5)
    #grid('on')
 #   ax3d.legend(numpoints=1,fontsize=14)
    fig3d.savefig('../figures/whole_system.png')
    show()
