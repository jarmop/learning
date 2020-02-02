import React, { useState } from 'react';
import './App.css';
import Flash from './Flash';

function App() {
  // Declare a new state variable, which we'll call "count"
  const [count, setCount] = useState(0);

  return (
    <div>
      <p>You clicked {count} times</p>
      <button onClick={() => setCount(count + 1)}>
        Click me
        </button>
      <Flash />
    </div>
  );
}

export default App;
