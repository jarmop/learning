function parseHeaderName(header) {
  if (header.includes('-')) {
    const nameParts = header.split('-')
    return nameParts[0] + nameParts[1][0].toUpperCase() + nameParts[1].substring(1)
  }

  return header
}

function extractHeaders(rawRequest) {
  return rawRequest.substring(4, rawRequest.substring(4).indexOf('*') + 3).split(';')
}

function extractBody(rawRequest) {
  const requestWithoutHeaders = rawRequest.split('\n')[1]
  return requestWithoutHeaders.substring(4, requestWithoutHeaders.substring(4).indexOf('*') + 4)
}

function parseBody(contentType, body) {
  if (contentType === 'json') return JSON.stringify(body)
  else return body
}

module.exports = {
  parseToRequest: function (data) {
    try {
      const headers = extractHeaders(data)

      let request = {
        headers: {},
      }

      headers.forEach((h) => {
        const [name, value] = h.split(':')
        request.headers[parseHeaderName(name)] = value
      })

      if (!request.headers.contentType) throw new Error('ContentType is undefined')

      const body = extractBody(data)
      if (body) {
        if (request.headers.contentType === 'json') request.body = JSON.parse(body)
        else request.body = body
      }

      return request
    } catch (err) {
      throw new Error('Bad Request!')
    }
  },

  parse: function (request) {
    return `****content-type:${request.headers.contentType};****\n****${parseBody(
      request.headers.contentType,
      request.body
    )}****`
  },

  setContentTypeHeader: function (body) {
    if (typeof body === 'string' || typeof body === 'boolean' || typeof body === 'number') {
      return 'text'
    } else if (typeof body === 'object') return 'json'
    else return 'unknown'
  },
}
