

f = open("cqt_factor_table_downsample8.csv")
rows= f.readlines()
f.close()

#original approach - sum all fft bins into a cqt bin
"""
for r in xrange(len(rows)):
    rows[r] = rows[r].replace('\n','')
    items = rows[r].split(',')
    #outline += "cqt[%d] = (w * cqt[%d]) +  ((1.0 - w) * cf[%d] * ("%(r,r,r)                        
    outline = "cqt[%d] = cf[%d] * ("%(r,r)    
    for i in xrange(len(items)):
        if items[i] != "0":
            
            if items[i] == "1":
                outline += "fftbins_array[%d]"%i + " + "
            else:
                outline += "(fftbins_array[%d]"%i + " * %s) + "%items[i]
    outline =  outline[:-2] + ");"
    #outline += "if (t > cqt[%d]) cqt[%d] = t;\n"%(r,r)    
    #outline += "else cqt[%d] = (w * cqt[%d]) +  ((1.0 - w) *t);\n"%(r,r)
    print outline
"""    

#new approach - take the max value from all fft bins into a cqt bin
for r in xrange(len(rows)):
    rows[r] = rows[r].replace('\n','')
    items = rows[r].split(',')
    #outline += "cqt[%d] = (w * cqt[%d]) +  ((1.0 - w) * cf[%d] * ("%(r,r,r)                        
    outline = "cqt[%d] = 0; "%(r)    
    for i in xrange(len(items)):
        if items[i] != "0":
            outline += "if(fftbins_array[%d] > cqt[%d]) "%(i,r) 
            outline += "cqt[%d] = fftbins_array[%d]; "%(r,i)

    #outline += "if (t > cqt[%d]) cqt[%d] = t;\n"%(r,r)    
    #outline += "else cqt[%d] = (w * cqt[%d]) +  ((1.0 - w) *t);\n"%(r,r)
    print outline