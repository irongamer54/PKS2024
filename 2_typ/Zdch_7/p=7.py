def cntrl(a,b):
    if (a==b):
        return a
    if (a > b) :
        tmp = a
        a = b
        b = tmp 
    return cntrl(a,b-a)


def new_key(p,q):
    s=p*q
    f=(p-1)*(q-1)
    k=1
    for i in range(f-1,0,-1):
        if cntrl(f,i)==1:
            #print(i)
            k=i
            break
    d=1
    while 1:
        if((d*k)%f==1 and d!=k):
            #print(d,k,s,f)
            break
        d+=1
    return [k,d,s]

keys=new_key(7,17)
k,d,s=keys[0],keys[1],keys[2]
M=10
B=(M**k)%s
I=(B**d)%s
print(B,I)



#print(cntrl(12,12))
