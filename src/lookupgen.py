import sys

with open(sys.argv[1]) as f:
    chartabl = {l.strip()[0]: i for i, l
                in enumerate(f.read().strip().split('\n'))}


lut = [chartabl.get(chr(i), -1) for i in range(0x21, 0x7f)]
print(lut)
LN = 15
print('{')
for lin in (lut[i:i + LN] for i in range(0, len(lut), LN)):
    print(f"\t{', '.join(map(str, lin))},")
print('}')
