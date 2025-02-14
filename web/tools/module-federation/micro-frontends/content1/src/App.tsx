import { useState } from 'react'
import './App.css'

// eslint-disable-next-line react-refresh/only-export-components
export default () => {
  const [count, setCount] = useState(0)

  return (
    <div className="card">
      <p>Counter from remote</p>
      <button onClick={() => setCount((count) => count + 1)}>
        count is {count}
      </button>
    </div>
  )
}
