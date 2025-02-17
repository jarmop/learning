const net = require('net')

// Establish a connection to the server
const client = net.createConnection({ host: '127.0.0.1', port: 3000 })

// Event listener for data received from the server
client.on('data', (data) => {
  console.log(data)
})

// Send data to the server
client.write('Hello! From client')
