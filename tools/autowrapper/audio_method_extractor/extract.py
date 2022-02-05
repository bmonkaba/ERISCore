
output_filename = "ErisParameterController.h"
output = open(output_filename,"w")

#templates
header = """#include "ErisAudio.h"
#include "SvcErisAudioParameterController.h"

void FLASHMEM SvcErisAudioParameterController::messageHandler(AppBaseClass *sender, const char *message){
    char c[64];
    char i[64];
    char m[64];
    char p[64];
    char objName[64];

    int total_read;
    //class,instance,method,params
    sci->printf("M ");sci->println(message);

    total_read = sscanf(message, "%s %s %s %s" , c, i, m, p);
    sci->printf("M APC Request: class: %64s instance: %64s method:%64s params: %64s" , c, i, m, p);//"%64s %64s %64s %64s" , c, i, m, p
    sci->send();
    if(0){}"""
    
class_code_template = \
"""else if(strncmp(gPC_{GLOBAL_CLASS_STR},c,sizeof(gPC_{GLOBAL_CLASS_STR})) == 0 &&\\
        strncmp(gPM_{METHOD},m,sizeof(gPM_{METHOD})) == 0){
        strcpy(objName,{CLASS}::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        {INNER_CODE}
    }"""

inner_code_template = \
"""{CLASS}* eac = ({CLASS}*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
{VARS}
            total_read = sscanf(p, "{SCAN}" , {ARGS_ADDR}); // {NATIVE_TYPE}
            eac->{METHOD}({ARGS});
        }"""

footer = "}\n"


#containers to capture the data to generate the global strings
PC = [] #audio component classes
PM = [] #audio component methods


    
def buildParameterString(a):
    ret = ""
    scan = ""
    args = ""
    
    for p in a:
        if p.find("-")>0:
            t = p.split("-")[0]
            n = p.split("-")[1]
            args += "&" + n + ","
            if t == "uint8_t":
                ret += "\t\t\tuint8_t " + n + ";\n"
                scan += "%c,"
            if t == "int8_t":
                ret += "\t\t\tint8_t " + n + ";\n"
                scan += "%d,"
            
            if t == "uint16_t":
                ret += "\t\t\tuint16_t " + n + ";\n"
                scan += "%hu,"    
            if t == "int16_t":
                ret += "\t\t\tint16_t " + n + ";\n"
                scan += "%hi,"
                
            if t == "uint32_t":
                ret += "\t\t\tuint32_t " + n + ";\n"
                scan += "%lu,"
            if t == "int32_t":
                ret += "\t\t\tint32_t " + n + ";\n"
                scan += "%li,"
            
            if t == "int":
                ret += "\t\t\tint16_t " + n + ";\n"
                scan += "%f,"
                
            if t == "float":
                ret += "\t\t\tfloat " + n + ";\n"
                scan += "%f,"
            if t == "float16_t":
                ret += "\t\t\tfloat16_t " + n + ";\n"
                scan += "%f,"
            if t == "float32_t":
                ret += "\t\t\tfloat32_t " + n + ";\n"
                scan += "%f,"
            if t == "double":
                ret += "\t\t\tint32_t " + n + ";\n"
                scan += "%Lf,"
            if t == "bool":
                ret += "\t\t\tbool " + n + ";\n"
                scan += "%c,"
                
    return [ret[:-1],scan[:-1],args[:-1]]
            
def autocode(r,c,m,a):
    s = str([c,m,a])
    s = s.replace('Audio','erisAudio')
    s = s.replace('eriseris','eris')
    print(s + ", \\")
    c = "eris" + c  #framework name
    ret = class_code_template
    ret = ret.replace("{GLOBAL_CLASS_STR}",c.upper())
    ret = ret.replace("{CLASS}",c)
    ret = ret.replace("{METHOD}",m.upper())
    PC.append(c)
    PM.append(m)
    (var,scan,args) = buildParameterString(a)
    inner = inner_code_template
    inner = inner.replace("{SCAN}",scan)
    inner = inner.replace("{ARGS_ADDR}",args)
    args = args.replace("&","")
    inner = inner.replace("{ARGS}",args)
    if len(args) < 1:
        inner = inner.replace("total_read","//total_read")
        inner = inner.replace("{VARS}","")
    else:
        inner = inner.replace("{VARS}",var)
    inner = inner.replace("{CLASS}",c)
    inner = inner.replace("{METHOD}",m)
    #ret = ret.replace("{INNER_CODE}","//"+str(a).replace("-"," ")+"\n{INNER_CODE}")
    inner = inner.replace("{NATIVE_TYPE}",str(a).replace("-"," "))
    ret = ret.replace("{INNER_CODE}",inner)
    #ret += "\t}\n"
    
    #remove any blank lines
    ret = "\n".join([s for s in ret.split('\n') if s])
    output.write(ret)
    #print(ret)
    return
    

#
# extract the return type, class, method, and args the call the above autocode function
#

from os import walk

f = []
m = {}

filepath = ".\inc"
for (dirpath, dirnames, filenames) in walk(filepath):
    f.extend(filenames)
    break

output.write (header)

for file in filenames:
    f = open(filepath + "\\" + file,'r')
    try:
        lines = f.readlines()
    except:
        print ("cant read " + file)
    finally:
        f.close()
        for line in lines:
            line = line.replace('AudioFilterStateVariable:','AudioFilterStateVariable')
            line = line.replace('AudioFilterLadder:','AudioFilterLadder')
            line = line.replace(':','')
            line = line.replace(':','')
            line = line.replace('const','')
            line = line.replace("unsigned int","uint32_t")
            #print(line)
            
            if line.find("update")>0 \
                or line.find("static")>-1 or line.find("inline")>-1 or \
                line.find("*")>-1 or line.find("&")>-1 :
                 pass
            elif line.find("class")==0:
                s = line.split('class')[1].split(' ')[1].strip()
                if len(s) > 2:
                    audio_class = s
                    #print(len(s))
            elif line.find("(") > 0 and (line.find("void") > 0 or \
             line.find("bool") > 0 or line.find("uint32_t") > 0 or \
             line.find("int32_t") > 0):
                audio_method = line.split('(')[0].lstrip().split(" ")[-1]
                if audio_class != audio_method and audio_method.find(':')<0:
                    audio_return_type = line.split('(')[0].lstrip().split(" ")[0]
                    if audio_return_type.find("memcpy") < 0 and audio_return_type.find("friend") < 0 and audio_return_type.find("//") < 0 and len(audio_method) > 2:
                        audio_args = line.split('(')[1].split(')')[0].replace(', ',',').replace(' ','-').split(',')
                        autocode(audio_return_type,audio_class,audio_method,audio_args)

output.write (footer)

#write out the globals

# using list comprehension
# remove duplicated items from the lists
res = []
[res.append(x) for x in PC if x not in res]
PC = res
res = []
[res.append(x) for x in PM if x not in res]
PM = res

for item in PC:
    output.write("const char gPC_"+ item.upper() + "[] PROGMEM = \""+ item +"\";\n")

for item in PM:
    output.write("const char gPM_"+ item.upper() + "[] PROGMEM = \""+ item +"\";\n")

output.close()

