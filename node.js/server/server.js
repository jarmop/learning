const http = require('http');
const fs = require('fs');
const opn = require('opn');

http.createServer((request, response) => {
  fs.readFile('index.html', (error, data) => {
    response.write(data);
    response.end();
  });
}).listen('8080', () => {
  opn('http://localhost:8080/')
});