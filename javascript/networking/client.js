const { connectToServer } = require('./protocol88/protocol88')

const client = connectToServer('127.0.0.1', 3900)

client.send('Hello! from client')

client.on('response', (response) => {
  console.log(response)
})
