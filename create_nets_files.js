const fs = require('fs');
const nets = require('./visualisation/libraries/example-nets').nets

Object.entries(nets).forEach(([key, value]) => {
    const data = JSON.stringify(value, null, 4);
    fs.writeFile(`./time-leak-detector/nets/${key}.json`, data, err => {
        if (err) {
            throw err;
        }
    });
});