const http = require('http');

//create a server object:
http.createServer((request, response) => {
  response.write('Hello World!'); //write a response to the client
  response.end(); //end the response
}).listen(8080); //the server object listens on port 8080