import random
import time
while True:
    time.sleep(.25)
    nums = [random.randint(700000, 3000000) for x in range(4)]
    output = ':'.join(str(x) for x in nums)
    #output = str(nums[0])+":"+str(nums[1])+":"+str(nums[2])+":"+str(nums[3])
    #print(output)
    f = open('cpu_data.txt', 'a')
    f.write(output+"\n")
    f.close()

