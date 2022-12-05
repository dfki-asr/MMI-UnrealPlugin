import glob, re

def replace_string(file, replacements):
    print("process ", file)
    with open(file, "rt") as fin:
        data = fin.read()
        fin.close()
    for(input, output) in replacements:
        data = re.sub(input, output, data)
    with open(file, "wt") as fout:
        fout.write(data)
        fout.close()



API = "MMIFRAMEWORK_API"
for f in glob.glob("Public/*.h"):
    replace_string(f, [(API, "")])
    replace_string(f, [
                    ("class (\S+) :", "class " + API + " \\1 :"), 
                    ("class (\S+) {", "class " + API + " \\1 {") ])
    

