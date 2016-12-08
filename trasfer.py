with open('pre_graph.txt') as f:
    tag=0
    for line in f:
        if tag == 0:
            v=int(line[:-1])
            tag += 1
        else:
            if tag == 1:
                g=[[0]*v for i in xrange(v)]
                tag += 1
            lk = [ int(e) for e in line[:-1].split(' ') if e != '' ]
            if len(lk) == 2:
                g[lk[0]][lk[1]]=1
                g[lk[1]][lk[0]]=1
            elif len(lk) == 3:
                g[lk[0]][lk[1]]=lk[2]
                g[lk[1]][lk[0]]=lk[2]
            elif len(lk) > 3:
                g[lk[0]][lk[1]]=lk[2]
                g[lk[1]][lk[0]]=lk[3]
with open('graph.txt','w') as f:
    print>>f, v
    for line in g:
        for a in line[:-1]:
            print>>f, a,
        print>>f,line[-1] 
