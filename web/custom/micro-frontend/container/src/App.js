import './App.css'
import React from 'react'

function App() {
  const name = 1
  const host = 'http://localhost:3001'
  React.useEffect(() => {
    const scriptId = `micro-frontend-script-${name}`

    if (document.getElementById(scriptId)) {
      this.renderMicroFrontend()
      return
    }

    fetch(`${host}/asset-manifest.json`)
      .then((res) => res.json())
      .then((manifest) => {
        const script = document.createElement('script')
        script.id = scriptId
        script.crossOrigin = ''
        script.src = `${host}${manifest.files['main.js']}`
        script.onload = renderMicroFrontend
        document.head.appendChild(script)
      })
  }, [])

  const renderMicroFrontend = () => {
    window[`render${name}`](`${name}-container`)
  }

  return (
    <div className="App">
      <div>Container</div>
      <div id={`${name}-container`}></div>
    </div>
  )
}

export default App
