const net = require('net')

// Create a TCP server instance
const server = net.createServer()

// Event listener for incoming connections
server.on('connection', (socket) => {
  // Event listener for data received from the client
  socket.on('data', (data) => {
    console.log(data)
  })
})

// Start the server, listening on port 3000 and IP address 127.0.0.1
server.listen(3000, '127.0.0.1', () => {
  console.log('Server is listening on ', server.address())
})
