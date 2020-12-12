const { json } = require("body-parser");
const { add } = require("lodash");

let currentMaxPlace = 2
let currentMaxHigh = 0
let currentMaxLow = 1

function generateNet(net = { "places": ["p1", "p2"], "transitions": { "high": [], "low": ["L1"] }, "flows": { "places": { "p1": ["L1"] }, "transitions": { "L1": ["p2"] } } }, recursionDepth = 0, distribution = [10, 10, 40, 20, 20], previous = {type: 1}) {
    switch (getType(distribution)) {
        case 1:
            returnNet = getBaseLow(net);
            break;
        case 2:
            returnNet = getBaseHigh(net);
            break;
        case 3:
            returnNet = getSequentialComposition(net, recursionDepth + 1, distribution);
            break;
        case 4:
            returnNet = getBranchingComposition(net, recursionDepth + 1, distribution, previous);
            break;
        case 5:
            returnNet = getParallelComposition(net, recursionDepth + 1, distribution, previous);
            break;
    }

    if (recursionDepth === 0) {
        return [returnNet];
    }
    return returnNet;
}

function getType(distribution) {
    const ranges = getRanges(distribution);
    const random = getRandom();
    for (let i = 0; i < ranges.length; ++i) {
        if (random >= ranges[i][0] && random <= ranges[i][1]) {
            return i + 1;
        }
    }
}

function getRandom() {
    return Math.floor(Math.random() * 100)
}

function getRanges(distribution) {
    const ranges = [];
    for (let i = 0; i < distribution.length; ++i) {
        if (i === 0) {
            ranges.push([0, distribution[i] - 1])
        } else {
            ranges.push([ranges[i - 1][1] + 1, ranges[i - 1][1] + distribution[i]])
        }
    }

    return ranges;
}

function getBaseHigh(net) {
    const netMax = getNetMax()

    net.places.push(`p${netMax.maxPlace + 1}`);
    net.transitions.high.push(`H${netMax.maxHigh + 1}`);

    net.flows.places[`p${netMax.maxPlace}`] = [`H${netMax.maxHigh + 1}`];
    net.flows.transitions[`H${netMax.maxHigh + 1}`] = [`p${netMax.maxPlace + 1}`];

    currentMaxHigh += 1;
    currentMaxPlace += 1;

    return net
}

function getBaseLow(net) {
    const netMax = getNetMax()

    net.places.push(`p${netMax.maxPlace + 1}`);
    net.transitions.low.push(`L${netMax.maxLow + 1}`);

    net.flows.places[`p${netMax.maxPlace}`] = [`L${netMax.maxLow + 1}`];
    net.flows.transitions[`L${netMax.maxLow + 1}`] = [`p${netMax.maxPlace + 1}`];

    currentMaxLow += 1;
    currentMaxPlace += 1;

    return net
}

function getSequentialComposition(net, recursionDepth, distribution) {
    if (recursionDepth > 10) {
        distribution = [50, 50, 0, 0, 0];
    }
    let netOutput = generateNet(net, recursionDepth, distribution, {type: 3});
    netOutput = generateNet(netOutput, recursionDepth, distribution, {type: 3});

    return netOutput;
}


function getBranchingComposition(baseNet, recursionDepth, distribution, previous) {
    if (recursionDepth > 2) {
        distribution = [50, 50, 0, 0, 0]
    }

    let startPlace = baseNet.places[baseNet.places.length - 1];
    if (previous.type === 4) {
        startPlace = previous.startPlace;
    }

    const branchOne = getBranch(startPlace, recursionDepth, distribution, previous);
    const branchTwo = getBranch(startPlace, recursionDepth, distribution, previous);


    if (previous.type !== 4) {
        baseNet = mergeNetTwoToOne(baseNet, branchOne);
        baseNet = mergeNetTwoToOne(baseNet, branchTwo);
        baseNet = mergeBranches(baseNet);
    } else {
        baseNet = mergeNetTwoToOne(branchOne, branchTwo);
    }

    return baseNet;

}

function getBranch(startPlace, recursionDepth, distribution, previous) {
    const branch = generateNet(getInitialNet(startPlace), recursionDepth, distribution, {type: 4, startPlace: startPlace});
    if (previous.type !== 4) {
        branch.places.shift();
    }

    return branch;
}

function mergeNetTwoToOne(base, second) {
    base.places = base.places.concat(second.places);
    base.transitions.low = base.transitions.low.concat(second.transitions.low);
    base.transitions.high = base.transitions.high.concat(second.transitions.high);

    Object.keys(second.flows.places).forEach(key => {
        if (!base.flows.places[key]) base.flows.places[key] = [];
        base.flows.places[key] = base.flows.places[key].concat(second.flows.places[key]);
    })

    Object.keys(second.flows.transitions).forEach(key => {
        if (!base.flows.transitions[key]) base.flows.transitions[key] = [];
        base.flows.transitions[key] = base.flows.transitions[key].concat(second.flows.transitions[key]);
    })

    return base;
}

function mergeBranches(net) {
    const placesWithNoOutput = getPlacesWithNoOutput(net)

    if (placesWithNoOutput.length > 1) {
        const newTransitions = []
        let newPlace = getNewPlace();
        net.places.push(newPlace);

        placesWithNoOutput.forEach(place => {
            const transition = getRandomTransition();
            newTransitions.push(transition);

            transition.charAt(0) === 'L' ? net.transitions.low.push(transition) : net.transitions.high.push(transition);
            net.flows.places[place] = [transition];
        });

        newTransitions.forEach(transition => {
            net.flows.transitions[transition] = [newPlace];
        })

        const lastTransition = getRandomTransition();
        lastTransition.charAt(0) === 'L' ? net.transitions.low.push(lastTransition) : net.transitions.high.push(lastTransition);
        net.flows.places[newPlace] = [lastTransition];

        newPlace = getNewPlace();
        net.places.push(newPlace);
        net.flows.transitions[lastTransition] = [newPlace];
    }

    return net;
}

function getPlacesWithNoOutput(net) {
    return net.places.filter(place => !net.flows.places[place] || net.flows.places[place].length === 0);
}

function getParallelComposition(baseNet, recursionDepth, distribution, previous) {
    if (recursionDepth > 2) {
        distribution = [50, 50, 0, 0, 0]
    }

    let parallelOne;
    let parallelTwo;
    if (previous.type === 5) {
        const startTransition = previous.startTransition;
        parallelOne = getParallel(startTransition, recursionDepth, distribution, {type: previous.type, startTransition: startTransition});
        parallelTwo = getParallel(startTransition, recursionDepth, distribution, {type: previous.type, startTransition: startTransition});
        baseNet = mergeNetTwoToOne(parallelOne, parallelTwo);
    } else {
        const baseEndPlace = baseNet.places[baseNet.places.length - 1];
        const startTransition = getRandomTransition();
        startTransition.charAt(0) === 'L' ? baseNet.transitions.low.push(startTransition) : baseNet.transitions.high.push(startTransition);
        baseNet.flows.places[baseEndPlace] = [startTransition];
        parallelOne = getParallel(startTransition, recursionDepth, distribution, {type: previous.type, startTransition: startTransition});
        parallelTwo = getParallel(startTransition, recursionDepth, distribution, {type: previous.type, startTransition: startTransition});
        baseNet = mergeNetTwoToOne(baseNet, parallelOne);
        baseNet = mergeNetTwoToOne(baseNet, parallelTwo);
        baseNet = mergeParallel(baseNet);
    }


    return baseNet;
}

function getParallel(startTransition, recursionDepth, distribution, previous) {
    const parallel = generateNet(getInitialNet(), recursionDepth, distribution, {type: 5, startTransition: previous.startTransition});
    const parallelStartPlace = parallel.places[0];


        if (!parallel.flows.transitions[startTransition]) parallel.flows.transitions[startTransition] = [];
        parallel.flows.transitions[startTransition] = parallel.flows.transitions[startTransition].concat([parallelStartPlace]);


    return parallel;
}

function mergeParallel(net) {
    const placesWithNoOutput = getPlacesWithNoOutput(net)

    if (placesWithNoOutput.length > 0) {
        const newTransition = getRandomTransition();
        newTransition.charAt(0) === 'L' ? net.transitions.low.push(newTransition) : net.transitions.high.push(newTransition);

        placesWithNoOutput.forEach(place => {
            net.flows.places[place] = [newTransition];
        })

        const newPlace = getNewPlace();
        net.places.push(newPlace);
        net.flows.transitions[newTransition] = [newPlace];
    }

    return net;
}

function getNetMax() {
    const netMax = {};

    netMax.maxPlace = currentMaxPlace;
    netMax.maxHigh = currentMaxHigh;
    netMax.maxLow = currentMaxLow;

    return netMax;
}

function getInitialNet(startPlace) {
    const netMax = getNetMax();
    let addedPlace = `p${netMax.maxPlace + 1}`

    if (!startPlace) {
        startPlace = `p${netMax.maxPlace + 1}`;
        addedPlace = `p${netMax.maxPlace + 2}`;
        currentMaxPlace += 1;
    }

    if (getType([50, 50, 0, 0, 0]) === 1) {
        const net = { "places": [startPlace, addedPlace], "transitions": { "high": [], "low": [`L${netMax.maxLow + 1}`] }, "flows": { "places": {}, "transitions": {} } }
        net.flows.places[startPlace] = [`L${netMax.maxLow + 1}`];
        net.flows.transitions[`L${netMax.maxLow + 1}`] = [addedPlace];
        currentMaxPlace += 1;
        currentMaxLow += 1;

        return net;
    }

    const net = { "places": [startPlace, addedPlace], "transitions": { "high": [`H${netMax.maxHigh + 1}`], "low": [] }, "flows": { "places": {}, "transitions": {} } }
    net.flows.places[startPlace] = [`H${netMax.maxHigh + 1}`];
    net.flows.transitions[`H${netMax.maxHigh + 1}`] = [addedPlace];
    currentMaxPlace += 1;
    currentMaxHigh += 1

    return net;
}

function getRandomTransition(creteNew = true) {
    const netMax = getNetMax();
    if (getType([50, 50, 0, 0, 0]) === 1) {
        currentMaxLow += 1;
        return `L${netMax.maxLow + 1}`
    }

    currentMaxHigh += 1;
    return `H${netMax.maxHigh + 1}`;
}

function getNewPlace() {
    const netMax = getNetMax();
    currentMaxPlace += 1;

    return `p${netMax.maxPlace + 1}`;
}

module.exports = generateNet;