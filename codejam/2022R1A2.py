t=int(input())

arr=[512, 1, 2, 1024, 4, 2048, 4096, 32768, 8, 131072, 262144, 524288, 1048576, 9, 10, 11, 16, 3, 12, 14, 15, 17, 16384, 18, 19, 20, 5, 21, 27, 28, 26, 16777216, 32, 33554432, 67108864, 134217728, 268435456, 536870912, 6, 7, 33, 34, 36, 37, 38, 39, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 64, 59, 13, 60, 61, 62, 65536, 63, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 2097152, 22, 23, 24, 25, 128, 29, 30, 31, 4194304, 35, 40, 256, 8192, 8388608]
s=1073744546
print(len(arr))
m=(1073741823)
for case in range(t):
    n=int(input())
    cs=s
    for i in arr:
        print(i,end=" ",flush=True)
    arr2=list(map(int,input().split()))
    cs+=sum(arr2)
    p=1
    res=[]
    cs>>=1
    arr2.sort()
    ind=99
    while cs>m:
        cs-=arr2[ind]
        res.append(arr2[ind])
        ind-=1
    while cs:
        if cs&1:
            res.append(p)
        cs>>=1
        p<<=1
    for i in res:
        print(i,end=" ",flush=True)
    
    
    
    
        