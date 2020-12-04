from subprocess import call
from Naked.toolshed.shell import muterun_js
import subprocess, os, numpy
import matplotlib.pyplot as plt
import random
from shutil import copyfile

ROOT = './netPiecesPrunable/'
outputFile = 'output.json'

def initializeOutputNet(): 
    netNum = str(random.randint(1,9))
    copyfile(ROOT + netNum + ".json", "./" + outputFile)

def appendRandomNet():
    netNum = str(random.randint(1,9))
    output = muterun_js('join-nets.js ' + './' + outputFile + " " + ROOT + netNum + ".json")
    output = output.stdout.decode("utf-8")[0:-1].split('\n')
    return int(output[0])

def runBenchmark(maxNoOfElements):
    initializeOutputNet()

    numberOfElements = 0
    results = [[[0],[0]], [[0],[0]], [[0],[0]], [[0],[0]]]

    while (numberOfElements < maxNoOfElements):
        numberOfElements = appendRandomNet()

        output2 = subprocess.check_output(['main.exe', 'output-run.json']).decode("utf-8")[0:-2].split(' ')
        if output2[0] == '':
            output2[0] = '0'

        for i in range(0, len(output2)):
            results[i][0].append(numberOfElements)
            results[i][1].append(int(output2[i]))

    return results

def plotResults(results, maxNoOfElements):
    step = 6
    creationDuration, noPruningNoConditional, pruningNoConditional, noPruningConditional = results

    line = numpy.linspace(0, maxNoOfElements, int(maxNoOfElements / step))

    creation = numpy.poly1d(numpy.polyfit(creationDuration[0], creationDuration[1], 2))
    nPnC = numpy.poly1d(numpy.polyfit(noPruningNoConditional[0], noPruningNoConditional[1], 2))
    PnC = numpy.poly1d(numpy.polyfit(pruningNoConditional[0], pruningNoConditional[1], 2))
    nPC = numpy.poly1d(numpy.polyfit(noPruningConditional[0], noPruningConditional[1], 2))

    plt.figure(1)
    plt.title('Creation duration')
    plt.xlabel('Number of elements')
    plt.ylabel('time [micro s]')

    plt.scatter(creationDuration[0], creationDuration[1], s=1, label='Creation scatter')
    plt.plot(line, creation(line), '-b', label="Creation duration")
    plt.legend(loc='best')


    plt.figure(2)
    plt.title('Pruning Comparison')
    plt.xlabel('Number of Elements')
    plt.ylabel('time [micro s]')

    plt.scatter(noPruningNoConditional[0], noPruningNoConditional[1], c='b', label='No prunning no conditional scatter',  s=1)
    plt.plot(line, nPnC(line), '-b', label="No pruning no conditional")
    plt.scatter(pruningNoConditional[0], pruningNoConditional[1], c='r', s=1, label='Prunning no conditional scatter')
    plt.plot(line, PnC(line), '-r', label="Pruning no conditional")
    plt.legend(loc='best')

    plt.figure(3)
    plt.title('Conditional comparison')
    plt.xlabel('Number of Elements')
    plt.ylabel('time [micro s]')

    plt.scatter(noPruningNoConditional[0], noPruningNoConditional[1], c='b', s=1, label='No prunning no conditional scatter')
    plt.plot(line, nPnC(line), '-b', label="No pruning no conditional")
    plt.scatter(noPruningConditional[0], noPruningConditional[1], c='g', label='No prunning conditional scatter', s=1)
    plt.plot(line, nPC(line), '-g', label="No pruning conditional")
    plt.legend(loc='best')



    plt.show()

def main():
    maxNoOfElements = 8000

    results = runBenchmark(maxNoOfElements)
    plotResults(results, maxNoOfElements)


main()