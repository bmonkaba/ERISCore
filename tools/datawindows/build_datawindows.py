import scipy.signal as s
import numpy

win = [numpy.blackman(1024).tolist()
     ,numpy.hamming(1024).tolist()
     ,numpy.kaiser(1024,14)]

for w in win:
    print("---")
    for i in [1024-32,1024-64,1024-128,1024-512]:
        for error in range(10):
            e = float(error)/100000.0
            if (s.check_COLA(w,1024,i,e) == True):
                print(e,1024 - i)
                break


