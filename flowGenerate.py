import sys
from random import randint

line= "cookie=0x00, duration=0s, table=0, n_packets=0, n_bytes=0, priority=2,ip,in_port=%d,dl_src=%s,nw_src=%s,nw_dst=%s actions=output:%d"
def ipTOint(s):
    b=s.split('.')
    rs=0
    for i in b:
        rs =int(i)+rs*2**8
    return rs

def intTOmac(i):
    b=""
    for e in [5,4,3,2,1,0]:
        t = i/256**e
        i = i - t*256**e
        t1 = t/16
        t2 = t%16
        f = lambda x:  x>9 and chr( ord('a')+x-10) or str(x)
        b += f(t1)+f(t2)+":"
    return b[:-1]


def intTOip(i):
    b=""
    for e in [3,2,1,0]:
        t = i/256**e
        i = i - t*256**e
        b += str(t)+"."
    return b[:-1]

def preline(ipt,mac,src,dst,opt):
    return line%(ipt,mac,src,dst,opt)

def generator(num):
    for i in xrange(num):
        src=10*2**24+2+i
        dst=10*2**24+randint(2,2**24-1)
        src = intTOip(src)
        dst = intTOip(dst)
        ipt=randint(1,10)
        opt=randint(1,10)
        mac=intTOmac(randint(1,2**28-1))
        print preline(ipt,mac,src,dst,opt)

generator(int(sys.argv[1]))
