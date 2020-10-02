const net1 = {
    places: ["p1", "p2", "p3", "p4"],
    transitions: ["t1", "t2", "t3", "t4", "t5"],
    flows: {
        places: {
            p1: ["t1"],
            p2: ["t2", "t3", "t4"],
            p3: ["t5"]
        },
        transitions: {
            t1: ["p2"],
            t2: ["p3"],
            t3: ["p3"],
            t4: ["p3"],
            t5: ["p4"]
        }
    },
};
const net2 = {
    places: ["p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8"],
    transitions: ["t1", "t2", "t3", "t4", "t5", "t6"],
    flows: {
        places: {
            p1: ["t1"],
            p2: ["t2"],
            p3: ["t3"],
            p4: ["t4"],
            p5: ["t5"],
            p6: ["t6"],
            p7: ["t6"]
        },
        transitions: {
            t1: ["p2", "p3"],
            t2: ["p4", "p5"],
            t3: ["p7"],
            t4: ["p6"],
            t5: ["p6"],
            t6: ["p8"]
        }
    },
};
const net3 = {
    places: ["p1", "p2", "p3", "p4", "p5"],
    transitions: ["t1", "t2"],
    flows: {
        places: {
            p1: ["t1"],
            p2: ["t2"],
            p3: ["t2"],
            p4: ["t2"]
        },
        transitions: {
            t1: ["p2", "p3", "p4"],
            t2: ["p5"]
        }
    }
};
