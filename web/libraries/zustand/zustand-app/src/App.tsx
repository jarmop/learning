import './App.css'
import { useStore } from './store'


function BearCounter() {
  const bears = useStore((state) => state.bears)
  return <h1>{bears} around here...</h1>
}

function Controls() {
  const increasePopulation = useStore((state) => state.increasePopulation)
  return <button onClick={increasePopulation}>one up</button>
}

function App() {
  return (
    <div>
      <BearCounter />
      <Controls />
    </div>
  )
}

export default App
