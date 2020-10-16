const fs = require('fs');
const nets = require('./visualisation/libraries/example-nets').nets

if (!fs.existsSync('time-leak-detector/nets')) {
    fs.mkdirSync('time-leak-detector/nets', { recursive: true });
}
Object.entries(nets).forEach(([key, value]) => {
    if (value.results.length > 0) {
        const data = JSON.stringify(value, undefined, 4);
        fs.writeFile(`./time-leak-detector/nets/${key}.json`, data, err => {
            if (err) {
                throw err;
            }
        });
    }
});