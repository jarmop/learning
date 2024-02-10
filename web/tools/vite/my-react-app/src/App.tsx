import { useReducer } from 'react'
import { MemoizedRow } from './MemoizedRow'
import { DispatchContext, reducer } from './reducer'

const initialData: string[][] = new Array(10).fill(new Array(6).fill('foo'))

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
  const [state, dispatch] = useReducer(reducer, { data: initialData })

  return (
    <DispatchContext.Provider value={dispatch}>
      <table>
        <tbody>
          {state.data.map((row, i) => (
            <MemoizedRow key={i} row={row} i={i} />
          ))}
        </tbody>
      </table>
    </DispatchContext.Provider>
  )
}

export default App
