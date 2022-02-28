#build a font sample sheet
import os

h_tag = """<p>{S}</p>"""
img_tag = """<img src={S} alt="font image"><BR>"""

f = open("index.html","w")
f.write("<html>")
f.write("""<body style="background-color:grey;">""")


for w in os.walk("."):
    if len(w[0])> 1 and w[0].find("FONT LICENCE") < 0:
        print (w[0])
        f.write(h_tag.replace("{S}",w[0]))
        f.write(img_tag.replace("{S}",w[0]+"/12.png"))
        f.write(img_tag.replace("{S}",str(w[0])+"/18.png"))
        f.write(img_tag.replace("{S}",str(w[0])+"/24.png"))

f.write("</body>")
f.write("</html>")
f.close()
            


