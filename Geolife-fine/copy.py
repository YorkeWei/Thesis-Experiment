import random
import os
#res = random.sample(range(1, 18209), num)
#for id in res:
    #fsize = os.path.getsize(".\\T-drive-processed-2\\8574.txt")
    #print(fsize / 1024)
    #os.system("copy .\\T-drive-processed-2\\" + str(id) + ".txt .\\T-drive-processed-80\\")
sum = 0
for i in range(1, 18209):
    sum += os.path.getsize("./geoprocessed-2/" + str(i) + ".txt")
pre = []
cnt_sum = 0
while cnt_sum < sum * 0.8:
    cnt = random.randint(1, 18208)
    while cnt in pre:
        cnt = random.randint(1, 18208)
    pre.append(cnt)
    cnt_sum += os.path.getsize("./geoprocessed-2/" + str(cnt) + ".txt")
    #cnt_sum += 1
    os.system("cp ./geoprocessed-2/" + str(cnt) + ".txt ./geoprocessed-80/")