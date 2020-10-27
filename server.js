const express = require('express');
const childProcess = require('child_process');
const app = express();
const bodyParser = require('body-parser');
const fs = require('fs');
const PORT = 5000;
const redirectToHTTPS = require('express-http-to-https').redirectToHTTPS;

app.use(redirectToHTTPS([/localhost:(\d{4})/],[],301));

app.use(bodyParser.json());
app.use(bodyParser.urlencoded());
app.use(express.static(__dirname + '/visualisation'));

app.post("/analyze", (request, response) => {
    let results = childProcess.execFileSync(`${__dirname}/time-leak-detector/main.exe`, [JSON.stringify(request.body.custom)]).toString().split('\n');
    console.log(results);
    results = results.map(result => result.replace('\r', ''));
    const resultString = JSON.parse(JSON.stringify(request.body.custom));
    delete resultString['results'];
    resultString.colors = {};
    resultString.conditional = [];
    results.forEach(result => {
        if (result.includes('-')) {
            const res = result.split('-');
            console.log(res);
            if (res && res[0].startsWith('H')) {
                resultString.colors[res[0]] = getColorFromTransitionType(res[1]);
                if (res[2] && res[2] == "conditional") {
                    resultString.conditional.push(res[0]);
                }
            }
        }
        
    });
    response.send(resultString);
});

app.get("/nets", (request, response) => {
    let nets = fs.readdirSync(`${__dirname}/time-leak-detector/nets`);
    
    nets = nets.map((net) => {
        const n = net.split('.');
        return {
            order: Number(n[0]),
            value: `${n[0]}.${n[1]}.${n[2]}`
        };
    });

    nets = nets.sort((n1, n2) => {
        if(n1.order > n2.order) {
            return 1;
        }
        return -1;
    });

    nets = nets.map(net => {
        return {
            "value": net.value,
            "text": net.value.replace('.json', '').replace(/-/g, ' ')
        };
    });

    response.send(nets);
});

app.get("/nets/:netFile", (request, response) => {
    const file = JSON.parse(fs.readFileSync(`${__dirname}/time-leak-detector/nets/${request.params.netFile}`));
    delete file['results'];
    response.send(file);
})



app.listen(PORT, () => {
    console.log(`Server started on port ${PORT}`)
});

const getColorFromTransitionType = (transitionType) => {
    if (transitionType.includes('-'))
    {
        return transitionType.split('-').map(color => colorTransitionMap[color]);
    }

    return colorTransitionMap[transitionType];
}

const colorTransitionMap = {
    "low": "red",
    "lowEnd": "magenta",
    "lowStart": "orange",
    "maxDuration": "green",
    "parallel": "purple"
}