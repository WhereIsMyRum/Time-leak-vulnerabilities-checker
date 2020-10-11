const express = require('express');
const process = require('child_process');
const app = express();
const bodyParser = require('body-parser');
const fs = require('fs');
const PORT = process.env.PORT;




app.use(bodyParser.json());
app.use(bodyParser.urlencoded());
app.use(express.static(__dirname + '/visualisation'));

app.post("/analyze", (request, response) => {
    //console.log(request.body.custom);
    //`${__dirname}/time-leak-detector/nets/net1.json`
    let results = process.execFileSync(`${__dirname}/time-leak-detector/main.exe`, [JSON.stringify(request.body.custom)]).toString().split('\r\n');
    const resultString = JSON.parse(JSON.stringify(request.body.custom));
    resultString.colors = {};
    results.forEach(result => {
        const res = result.split('-');
        if (res && res[0].startsWith('H')) {
            if (res[1] == 'low') resultString.colors[res[0]] = 'red';
            else if (res[1] == 'lowEnd') resultString.colors[res[0]] = 'magenta';
            else if (res[1] == 'lowStart') resultString.colors[res[0]] = 'orange';
        };
    });
    response.send(resultString);
});

app.get("/nets", (request, response) => {
    let nets = fs.readdirSync(`${__dirname}/time-leak-detector/nets`);
    nets = nets.map(net => {
        return {
            "value": net,
            "text": net.replace('.json', '').replace(/([a-z])([A-Z])/g, '$1 $2')
        };
    });
    response.send(nets);
});

app.get("/nets/:netFile", (request, response) => {
    response.sendFile(`${__dirname}/time-leak-detector/nets/${request.params.netFile}`);
})



app.listen(PORT, () => {
    console.log(`Server started on port ${PORT}`)
});