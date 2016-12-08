#!/usr/bin/python

# input := Graph, Rules
# Set V indicates the switches, Set E indicates the links

class NWGraph(object):

    """NWGraph has many class function
    First, use loadGraph to load the graph description file
    """

    def __init__(self):
        """TODO: to be defined1. """
        self.E = ()
        self.G = ()
        self.V = 0
        pass

    def loadGraph(self, filenm):
        """Load the description file

        :filenm: file name
        :returns: Config class vars, no return;

        """
        Ei=[]
        with open('graph.txt') as f:
            tag=True
            for line in f:
                if tag:
                    self.V=int(line[:-1])
                    tag = False
                else:
                    Ei +=[ [ int(e) for e in line[:-1].split(' ') if e != '' ] ]
        Ei = [ tuple(a) for a in Ei ]
        Ei = tuple(Ei)
        self.E = Ei
        self.G = (self.V,self.E)
        pass


        

    def Vnext(self,i):
        """
        return vertex and length
        """
        Vi,Ei=self.G
        for v,e in enumerate(Ei[i]):
            if e != 0:
                yield (v,e)

    def isVnext(self,i,ni):
        """TODO: return whether the i is point to ni.

        :Gi: graph
        :i: source vertex
        :ni: destined vertex
        :returns: boolean

        """
        for v,e in Vnext(Gi,i):
            if v == ni:
                return True
        return False


    def dijkstra(self,vxi):
        """TODO: source vertex is vxi. (meaning for vxi to other vertex)
        To find out the shortest paths
        :return: preview node list, next node list, and length of the path.
        """
        Vi,Ei = self.G
        sup=2**31
        pre=[ [] for a in xrange(Vi) ]
        nxt=[ [] for a in xrange(Vi) ]
        lgth=[sup]*Vi
        lgth[vxi]=0; 
        record=[vxi]
        while(len(record)<=Vi):
            e = record[-1]
            for ve,le in self.Vnext(e):
                nlgth = lgth[e]+le
                if nlgth < lgth[ve]:
                    lgth[ve] = nlgth
                    pre[ve] = [e]
                elif nlgth == lgth[ve]:
                    pre[ve] += [e]
            minlen = sup; tmp=e
            for a in xrange(Vi):
                if a not in record:
                    if lgth[a] < minlen:
                        minlen = lgth[a]
                        tmp = a
            record += [tmp]
        for x2,e2 in enumerate(pre):
            for i2 in e2:
                nxt[i2].append(x2)
        return (pre,nxt,lgth)

    def dijkpath(self,v):
        """TODO: return all the shortest path;
        From source node v to the other nodes.
        """
        vi,ei=self.G
        paths=[]
        pre,nxt,lth = self.dijkstra(v)
        tmp=[[v]]; ctmp=[]
        for i in xrange(vi):
            if tmp == []:
                break
            for ei in tmp:
                ctmp += [ ei+[a] for a in nxt[ei[-1]] ]
            tmp = ctmp
            ctmp = []
            paths += tmp
        return paths


#G=loadGraph()
#a,b = dijkstra(G,5)
G=NWGraph()
G.loadGraph("graph.txt")
print G.dijkpath(0)
