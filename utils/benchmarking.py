from subprocess import call
from Naked.toolshed.shell import muterun_js
import subprocess, os, numpy
import matplotlib.pyplot as plt
import random



numberOfElements = 0

creationDuration = [[0],[0]]
pruningDuration = [[0],[0]]
noPruningNoConditional = [[0],[0]]
pruningNoConditional = [[0],[0]]
noPruningConditional = [[0],[0]]

maxNoOfElemenst = 6000
step = 6

while (numberOfElements < maxNoOfElemenst):
    netNum = str(random.randint(1,9))
    output = muterun_js('join-nets.js ' + './output.json' + " " + './netPiecesPrunable/' + netNum + ".json")
    output = output.stdout.decode("utf-8")[0:-1].split('\n')
    print(output)
    numberOfElements = int(output[0])

    creation, pruning, nPnC, PnC, nPC, PC = 0, 0 ,0, 0, 0, 0
    for i in range(0, 1) :
        output2 = subprocess.check_output(['main.exe', 'output-run.json']).decode("utf-8")[0:-2].split(' ')
        if output2[0] == '':
            output2[0] = '0'
        creation += int(output2[0])
        nPnC += int(output2[1])
        PnC += int(output2[2])
        nPC += int(output2[3])

    creationDuration[0].append(numberOfElements)
    creationDuration[1].append(creation)
    noPruningNoConditional[0].append(numberOfElements)
    noPruningNoConditional[1].append(nPnC)
    pruningNoConditional[0].append(numberOfElements)
    pruningNoConditional[1].append(PnC)
    noPruningConditional[0].append(numberOfElements)
    noPruningConditional[1].append(nPC)

line = numpy.linspace(0, maxNoOfElemenst, int(maxNoOfElemenst / step))

creation = numpy.poly1d(numpy.polyfit(creationDuration[0], creationDuration[1], 2))
nPnC = numpy.poly1d(numpy.polyfit(noPruningNoConditional[0], noPruningNoConditional[1], 1))
PnC = numpy.poly1d(numpy.polyfit(pruningNoConditional[0], pruningNoConditional[1], 1))
nPC = numpy.poly1d(numpy.polyfit(noPruningConditional[0], noPruningConditional[1], 1))

plt.plot(line, creation(line), label="creation")
plt.plot(line, nPnC(line), label="npnc")
plt.plot(line, PnC(line), label="pnc")
plt.plot(line, nPC(line), label="npc")
plt.title('Creation duration')
plt.xlabel('Number of elements')
plt.legend(["Creation", "No pruning no conditional", "Pruning no conditional", "No pruning conditional"])
plt.ylabel('time [ms]')
plt.show()

