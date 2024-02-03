import { useState, useEffect, useRef } from 'react'
import { MemoizedRow } from './MemoizedRow'
import { replaceAtIndex } from './util'

const initialData: string[][] = new Array(1000).fill(new Array(6).fill('foo'))

/**
 * Depends on the dimensions of the table whether memoizing each cell is needed. Memoizing
 * rows seems to be useful even if there aren't that many rows but lot's of cells. The cache
 * hit needs to be calculated for every single cell, and that makes it expensive even if not
 * that many cells end up rendered.
 * 
 * Other options:
 * - set row or cell editable on click
 * - render only visible cells (this may need to be done anyway on very large tables)
 */
function App() {
  const [data, setData] = useState(initialData)
  const dataRef = useRef(data)

  useEffect(() => {
    dataRef.current = data
  }, [data])

  return (
    <>
      <table>
        <tbody>
          {data.map((row, i) => (
            <MemoizedRow key={i} row={row} i={i} onChange={(row) => setData(replaceAtIndex(dataRef.current, i, row))} />
          ))}
        </tbody>
      </table>
    </>
  )
}

export default App
