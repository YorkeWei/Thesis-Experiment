import random
import os
#res = random.sample(range(1, 9943), num)
#for id in res:
    #fsize = os.path.getsize(".\\T-drive-processed-2\\8574.txt")
    #print(fsize / 1024)
    #os.system("copy .\\T-drive-processed-2\\" + str(id) + ".txt .\\T-drive-processed-80\\")
sum = 0
for i in range(1, 9943):
    sum += os.path.getsize("./T-drive-processed-2/" + str(i) + ".txt")
pre = []
cnt_sum = 0
while cnt_sum < sum * 0.8:
    cnt = random.randint(1, 9942)
    while cnt in pre:
        cnt = random.randint(1, 9942)
    pre.append(cnt)
    cnt_sum += os.path.getsize("./T-drive-processed-2/" + str(cnt) + ".txt")
    #cnt_sum += 1
    os.system("cp ./T-drive-processed-2/" + str(cnt) + ".txt ./T-drive-processed-80/")