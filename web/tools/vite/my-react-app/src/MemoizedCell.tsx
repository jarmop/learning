import { memo, useContext } from 'react'
import { DispatchContext } from './reducer'

interface CellProps {
  value: string
  i: number
  j: number
}

export const MemoizedCell = memo(({ value, i, j }: CellProps) => {
  const dispatch = useContext(DispatchContext)

  return (
    <td>
      <input
        type="text"
        value={value}
        onChange={(event) => dispatch({ type: 'changeCell', value: event.target.value, i, j })}
      />
    </td>
  )
})
