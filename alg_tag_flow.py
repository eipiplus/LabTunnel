import G_alg_lib as glib

G=glib.loadGraph()

def lsf(Gi,src,listp, pnext):
    """TODO: search for the longer path
    Gi is graph, src is source node,listp is the node list,pnext is the result
    return the listp which not find.
    """
    tag=True
    rst=[]
    for v,e in glib.Vnext(Gi,src):
        if v in listp:
            tag=False
            pnext[v]=src
            listp.remove(v)
            rst=lsf(Gi,v,listp,pnext)
    if tag:
        return listp
    else:
        return rst

def FindinTag(p,tag):
    """To figure out wheather the path(p) is in the tag.

    :p: path [s,v1,v2...,d]
    :tag: [next(0),next(1),next(2)...,next(v-1)]
    :returns: IN(True)

    """
    for x,e in enumerate(p[:-1]):
        if tag[e]!=p[x+1]:
            return False
    return True

def Pex2T(Gi,pe):
    """TODO : transfer a path to a tag(path), with extend path

    :pe: Path data struct:[v2,v1,v3,v4]
    :returns: tage, Tag data struct:[next(v0),next(v1),next(v2)...]

    """

    v,e=Gi
    qe = [e for e in xrange(v) if e not in pe]
    tage=[-2]*v
    for x,a in enumerate(pe[:-1]):
        if(glib.isVnext(Gi,a,pe[x+1])):
            tage[a]=pe[x+1]
        else:
            print "Error, This path",pe,"is not in the graph."
            return []
    tage[pe[-1]]=-1
    for vi in pe:
        if qe != []:
            print vi,'#',lsf(Gi,vi,qe,tage)
    return tage

def TagPath(Gi,ps):
    taglist=[]
    tag_map=[]
    # sorted ffom big to small
    ps = sorted(ps,key=lambda x:len(x),reverse=True)
    # use the ps to generate the tree(or a line)
    for x,pe in enumerate(ps):
        flag=True
        for t,tage in enumerate(taglist):
            if FindinTag(pe,tage):
                tag_map[t] += [x]
                flag=False
        if flag:
            addn = Pex2T(Gi,pe)
            if addn!=[]:
                taglist += [addn]
                tag_map += [[x]]
    return (taglist,tag_map)

def PtoT2(Gi,pe):
    """TODO: Docstring for PtoT2.
    :Gi is the graph
    :pe is the path
    :returns: TODO

    """
    v,e=Gi
    tage=[-2]*v
    for x,a in enumerate(pe[:-1]):
        if(glib.isVnext(Gi,a,pe[x+1])):
            tage[a]=pe[x+1]
        else:
            print "Error, This path",pe,"is not in the graph."
            return []
    tage[pe[-1]]=-1
    return tage

def FindinTag2(pi,tagi):
    """To find out the path pi in tagi.
    If return true, the path i
    :pi: TODO
    :tagi: TODO
    :returns: TODO

    """
    collect=[]
    for x,e in enumerate(pi[:-1]):
        if tagi[e] == -2:
            collect.append(e)
            tagi[e] = pi[x+1]
        elif tagi[e] != pi[x+1]:
            for a in collect:
                tagi[a] = -2
            return False
    return True

def TagPath2(Gi,ps):
    """Tag Path, with the merged path, use FindinTag2 to merge path

    :Gi: TODO
    :ps: TODO
    :returns: TODO

    """
    taglist=[]
    tag_map=[]
    ps = sorted(ps,key=lambda x:len(x),reverse=True)
    for x,pe in enumerate(ps):
        flag=True
        for t,tage in enumerate(taglist):
            if FindinTag2(pe,tage):
                tag_map[t] += [x]
                flag=False
        if flag:
            addn = PtoT2(Gi,pe)
            if addn!=[]:
                taglist += [addn]
                tag_map += [[x]]
    return (taglist,tag_map)




