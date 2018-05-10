const WebSocket = require('ws');

const client = new WebSocket('ws://localhost:8080');

client.on('open', function open() {
  client.send('Hello from client!');
});

client.on('message', function incoming(data) {
  console.log('received: %s', data);
});