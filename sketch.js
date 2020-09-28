const side = 30;
const radius = 30;

function setup() {
    createCanvas(window.outerHeight, window.outerWidth);
}

function draw() {
    background(220);
    baseHeight = 200;
    baseLeft = 100;

    const net = {
        places: {
            p1: {
                transitions:['t1'],
                drawn:0,
                width:0,
                height:0
            },
            p2: {
                transitions:['t2'],
                drawn:0,
                width:0,
                height:0
            },
            p3: {
                transitions:['t3', 't4'],
                drawn:0,
                width:0,
                height:0
            },
            p4: {
                transitions:[],
                drawn:0,
                width:0,
                height:0
            }
        },
        transitions: {
            t1: {
                places:['p2'],
                drawn:0,
                width:0,
                height:0
            },
            t2: {
                places: ['p3'],
                drawn:0,
                width:0,
                height:0
            },
            t3: {
                places: ['p4'],
                drawn:0,
                width:0,
                height:0
            },
            t4: {
                places: [],
                drawn:0,
                width:0,
                height:0
            }
        },
        start: 'p1',
        marking: {
            'p1':1
        }
    };

    drawPlace(baseLeft, baseHeight, net.places[net.start], net);
}

function drawPlace(width, height, place, net, parentTransition) {
    if (!place.drawn) circle(width, height, radius);
    place.width = width;
    place.height = height;
    place.drawn = 1;
    if (parentTransition) drawArrow(createVector(parentTransition.width + side / 2, parentTransition.height), createVector(place.width - radius / 2, place.height));
    plusHeight = 0;
    if (place.transitions) {
        place.transitions.forEach(transition => {
            drawTransition(width + 100, height + plusHeight, net.transitions[transition], net, place);
            plusHeight += 10;
        });
    }
}

function drawTransition(width, height, transition, net, parentPlace) {
    if (!transition.drawn) square(width - side / 2, height - side / 2, side);
    transition.width = width;
    transition.height = height;
    transition.drawn = 1;
    drawArrow(createVector(parentPlace.width + radius / 2, parentPlace.height), createVector(transition.width - side / 2, transition.height));
    plusHeight = 0;
    if (transition.places) {
        transition.places.forEach(place => {
            drawPlace(width + 100, height + plusHeight, net.places[place], net, transition)
            plusHeight += 10
        })
    }
}

function drawArrow(base, vec) {
  push();
  line(base.x, base.y, vec.x, vec.y);
  refVec = createVector(0, 2*base.y);
  line(0,0, refVec.x, refVec.y)
  rotate();
  let arrowSize = 7;
  translate(vec.mag() - arrowSize, 0);
  triangle(0, arrowSize / 2, 0, -arrowSize / 2, arrowSize, 0);
  pop();
}
