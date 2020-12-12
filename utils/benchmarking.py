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
    copyfile("./baseNet.json", "./" + outputFile)

def appendRandomNet():
    output = muterun_js('join-nets.js ' + './' + outputFile)
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

    #scale creation to seconds
    creationDuration[1] = [element / 1000 for element in creationDuration[1]]
    noPruningNoConditional[1] = [element / 1000 for element in noPruningNoConditional[1]]
    pruningNoConditional[1] = [element / 1000 for element in pruningNoConditional[1]]
    noPruningConditional[1] = [element / 1000 for element in noPruningConditional[1]]

    creation = numpy.poly1d(numpy.polyfit(creationDuration[0], creationDuration[1], 2))
    nPnC = numpy.poly1d(numpy.polyfit(noPruningNoConditional[0], noPruningNoConditional[1], 2))
    PnC = numpy.poly1d(numpy.polyfit(pruningNoConditional[0], pruningNoConditional[1], 2))
    nPC = numpy.poly1d(numpy.polyfit(noPruningConditional[0], noPruningConditional[1], 2))

    plt.figure(1)
    plt.title('Comparison between creation duration and time leak detection duration')
    plt.xlabel('Number of elements')
    plt.ylabel('time [ms]')

    plt.scatter(creationDuration[0], creationDuration[1], c='r', s=1, label='Creation')
    plt.plot(line, creation(line), '-r', label="Creation")
    plt.scatter(noPruningNoConditional[0], noPruningNoConditional[1], c='b', label='Time leak detection',  s=1)
    plt.plot(line, nPnC(line), '-b', label="Time leak detection")
    plt.legend(loc='best')


    plt.figure(2)
    plt.title('Effect of pruning')
    plt.xlabel('Number of Elements')
    plt.ylabel('time [ms]')

    print(noPruningNoConditional)

    plt.scatter(noPruningNoConditional[0], noPruningNoConditional[1], c='b', label='Without pruning',  s=1)
    plt.plot(line, nPnC(line), '-b', label="Without pruning")
    plt.scatter(pruningNoConditional[0], pruningNoConditional[1], c='r', s=1, label='With pruning')
    plt.plot(line, PnC(line), '-r', label="With pruning")
    plt.legend(loc='best')

    plt.figure(3)
    plt.title('Effect of conditional analysis')
    plt.xlabel('Number of Elements')
    plt.ylabel('time [ ms]')

    plt.scatter(noPruningNoConditional[0], noPruningNoConditional[1], c='b', s=1, label='Without conditional')
    plt.plot(line, nPnC(line), '-b', label="Without conditional")
    plt.scatter(noPruningConditional[0], noPruningConditional[1], c='g', label='With conditional', s=1)
    plt.plot(line, nPC(line), '-g', label="With conditional")
    plt.legend(loc='best')

    plt.show()

def main():
    maxNoOfElements = 100

    results = runBenchmark(maxNoOfElements)
    plotResults(results, maxNoOfElements)


main()