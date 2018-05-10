const WebSocket = require('ws');

const server = new WebSocket.Server({ port: 8080 });

server.on('connection', function connection(ws) {
  ws.on('message', function incoming(message) {
    console.log('received: %s', message);
  });

  ws.send('Hello from server!');
});