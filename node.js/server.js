const http = require('http');
const fs = require('fs');

//create a server object:
http.createServer((request, response) => {
  fs.readFile('index.html', (error, data) => {
    response.write(data); //write a response to the client
    response.end(); //end the response
  });
}).listen(8080); //the server object listens on port 8080