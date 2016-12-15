
a=[1,2,3]
def resfunc(lt):
    if lt == []:
        return 0
    else:
        print a
        lt[:1] = []
        print lt
        return resfunc(lt)

print resfunc(a)
print 'a =',a
