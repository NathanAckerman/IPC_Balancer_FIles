#use python run_shells.py ./aff.c 16 1000 500
import sys
import subprocess
import os
import time

def main():
    if(len(sys.argv) != 6):
        print("Error: format is [python time_test.py num_tests program num_procs loop_length num_loops]")
        sys.exit(1)

    num_tests = sys.argv[1]
    cmd = " ".join(sys.argv[2:])
    
    print("num_tests: "+num_tests)
    print("cmd: "+cmd)
    print("starting tests")



    if os.path.exists("output.txt"):
        os.remove("output.txt")

    for i in range(int(num_tests)): 
        subprocess.check_output(cmd, shell=True)
    
    f = open("output.txt", "r")
    print(f)
    #print(f.read())


    total_real = 0
    total_user = 0
    total_sys = 0

    for x in f:
        print("\n\n******** Line: "+x+"**")
        try:
            splitt = x.split("\t")
            print(splitt)
            cat = splitt[0]
            time = splitt[1]
            print("cat: "+cat)
            print("time whole: "+time)
            
            arr = time.split("m")
            time_split = float(arr[0])*60 + float(arr[1].split("s")[0])

            if cat == "real":
                print("time split: "+ str(time_split))
                print(float(time_split))
                total_real += float(time_split)
                print("total*****"+str(total_real))
            elif cat == "user":
                print("time split: "+ str(time_split))
                total_user += float(time_split)
            elif cat == "sys":
                print("time split: "+ str(time_split))
                total_sys += float(time_split)

            else:
                print("other cat")
        
        except:
            print("skip empty line")


    ##############################
    print("total real: "+str(total_real))
    avg_real = total_real/float(num_tests)
    print("avg: "+str(avg_real))

    print("total user: "+str(total_user))
    avg_user = total_user/float(num_tests)
    print("avg: "+str(avg_user))

    print("total sys: "+str(total_sys))
    avg_sys = total_sys/float(num_tests)
    print("avg: "+str(avg_sys))

if __name__ == "__main__":
    main()
