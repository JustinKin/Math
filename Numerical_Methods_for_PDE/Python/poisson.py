#泊松方程-混合边界问题有限元求解，问题定义域为x:(-1,1),y:(-1,1)
#题目中未对角点进行定义，本程序中设定角点函数值为0
#于是在该区域均匀划分直角三角形线性元网格，即32个三角形单元，25个节点
#其中15个未知点，10个已知点
import numpy as np
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
nxmin = -1.; nxmax = 1.; nymin = -1.; nymax = 1.;
elenth = 0.5; S = elenth*elenth/2       #直角三角形单元边长及面积
nump = int((((nxmax-nxmin)/elenth+1))*(((nymax-nymin)/elenth+1)))#节点数
numele = int((nxmax-nxmin)/elenth*2*4)  #单元数
cord = np.zeros((nump,2), dtype = float)#定义节点坐标
trip = np.zeros((numele,3), dtype = int)#定义每个单元中的节点编号
aas = np.zeros((numele,3),dtype = float)#定义每个三角形单元中的a,b,c
bbs = np.zeros((numele,3),dtype = float)
ccs = np.zeros((numele,3),dtype = float)
#计算节点坐标
count = 0
for i in range(5):
    for j in range(5):
        cord[count] = np.array([nxmin+elenth*i,nymax-elenth*j])
        count += 1
#确定每个单元中的节点编号
count = 0
for i in range(4):
    for j in range(4):
        trip[count] = np.array([i+j*5,i+1+j*5,i+5+j*5], dtype = int)
        count += 1
        trip[count] = np.array([i+1+j*5,i+6+j*5,i+5+j*5], dtype = int)
        count += 1
#计算a,b,c
for i in range(numele):
    aas[i] = np.array([cord[trip[i,1],0]*cord[trip[i,2],1]-cord[trip[i,2],0]*cord[trip[i,1],1],cord[trip[i,2],0]*cord[trip[i,1],1]-cord[trip[i,1],0]*cord[trip[i,2],1],cord[trip[i,0],0]*cord[trip[i,1],1]-cord[trip[i,1],0]*cord[trip[i,0],1]])
    bbs[i] = np.array([cord[trip[i,1],1]-cord[trip[i,2],1],cord[trip[i,2],1]-cord[trip[i,0],1],cord[trip[i,0],1]-cord[trip[i,1],1]])
    ccs[i] = np.array([cord[trip[i,2],0]-cord[trip[i,1],0],cord[trip[i,0],0]-cord[trip[i,2],0],cord[trip[i,1],0]-cord[trip[i,0],0]])
pun = np.array([1,2,3,6,7,8,11,12,13,16,17,18,21,22,23],dtype = int) #内点编号
pno = np.array([0,4,5,9,10,14,15,19,20,24], dtype = int) #外点编号
#第一类边界条件
alpha = np.zeros((len(pno)), dtype = float)
#第二类边界条件
bsec = np.zeros((len(pun)))
bsec[0:3] = 1
bsec[-3:] = 1
beta = np.zeros((len(pun)))
beta[0:3] = 1.

#定义Ax=b线性方程组相关矩阵，该线性方程组由教材公式(2.7.15)得到，其中各项的计算式为(2.7.16)以及(2.7.17)，第二类边界问题的b向量由79页第一个方程计算
A = np.zeros((len(pun),len(pun)), dtype = float)
b = np.zeros((len(pun)), dtype = float)
for i in range(len(pun)):
    for j in range(len(pun)):
        asum = 0.
        for k in range(numele):
            if pun[j] in trip[k] and pun[i] in trip[k]:
                asum = asum + 1./4./S*(bbs[k,np.argwhere(trip[k] == pun[j])]*bbs[k,np.argwhere(trip[k] == pun[i])]+ccs[k,np.argwhere(trip[k] == pun[j])]*ccs[k,np.argwhere(trip[k] == pun[i])])
            A[i,j] = asum
for j in range(len(pun)):
    bsum1 = 0.
    bsum2 = 0.
    for i in range(numele):
        if pun[j] in trip[i]:
            bsum1 = bsum1 + 1./12.
    for i in range(len(pno)):
        for k in range(numele):
            if pun[j] in trip[k] and pno[i] in trip[k]:
                bsum2 = bsum2 + 1./4./S*(bbs[k,np.argwhere(trip[k] == pun[j])]*bbs[k,np.argwhere(trip[k] == pno[i])]+ccs[k,np.argwhere(trip[k] == pun[j])]*ccs[k,np.argwhere(trip[k] == pno[i])])*alpha[i]
    b[j] = bsum1 - bsum2 - bsec[j]*beta[j]*elenth
u = np.dot(np.linalg.inv(A),b)  #求解
for i in range(len(pun)):
    print('坐标',':',cord[pun[i]],',','函数值',':',u[i])
#画图
uplot = np.zeros((5,5),dtype = float)
uplot[0,1:4] = u[0:3]
uplot[1,1] = u[3]
uplot[1,2] = u[4]
uplot[1,3] = u[5]
uplot[2,1] = u[6]
uplot[2,2] = u[7]
uplot[2,3] = u[8]
uplot[3,1] = u[9]
uplot[3,2] = u[10]
uplot[3,3] = u[11]
uplot[4,1:4] = u[-3:]
fig = plt.figure()
ax3 = plt.axes(projection='3d')
xx = np.arange(-1,1.5,0.5)
yy = np.arange(-1,1.5,0.5)
X, Y = np.meshgrid(xx,yy)
ax3.plot_surface(X,Y,uplot,cmap = plt.get_cmap('brg'))
plt.show()

'''计算结果
坐标 : [-1.   0.5] , 函数值 : 1.8550420168067225
坐标 : [-1.  0.] , 函数值 : 2.398109243697479
坐标 : [-1.  -0.5] , 函数值 : 1.8550420168067223
坐标 : [-0.5  0.5] , 函数值 : 1.2610294117647056
坐标 : [-0.5  0. ] , 函数值 : 1.6911764705882348
坐标 : [-0.5 -0.5] , 函数值 : 1.2610294117647056
坐标 : [0.  0.5] , 函数值 : 0.9978991596638653
坐标 : [0. 0.] , 函数值 : 1.34453781512605
坐标 : [ 0.  -0.5] , 函数值 : 0.9978991596638652
坐标 : [0.5 0.5] , 函数值 : 0.8860294117647056
坐标 : [0.5 0. ] , 函数值 : 1.191176470588235
坐标 : [ 0.5 -0.5] , 函数值 : 0.8860294117647056
坐标 : [1.  0.5] , 函数值 : 0.8550420168067225
坐标 : [1. 0.] , 函数值 : 1.148109243697479
坐标 : [ 1.  -0.5] , 函数值 : 0.8550420168067224
'''