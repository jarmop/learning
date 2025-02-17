const { server } = require('./protocol88/protocol88')

server.startServer('127.0.0.1', 3900, (address) => {
  console.log('server is listening on ', address)
})

server.on('request', (request) => {
  console.log(request)
  server.send('Welcome!')
})

server.on('error', (error) => {
  console.log(error)
})
