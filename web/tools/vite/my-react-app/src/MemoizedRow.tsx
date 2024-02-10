import { memo } from 'react'
import { MemoizedCell } from './MemoizedCell'

interface RowProps {
  row: string[]
  i: number
}

export const MemoizedRow = memo(({ row, i }: RowProps) => {
  return (
    <tr>
      {row.map((col, j) => (
        <MemoizedCell key={`${i}-${j}`} value={col} i={i} j={j} />
      ))}
    </tr>
  )
})
