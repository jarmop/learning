import { memo } from 'react'
import { MemoizedCell } from './MemoizedCell'

interface RowProps {
  row: string[]
  i: number
  onChange: (row: string, j: number) => void
}

export const MemoizedRow = memo(({ row, i, onChange }: RowProps) => {
  return (
    <tr>
      {row.map((col, j) => (
        <MemoizedCell key={`${i}-${j}`} value={col} onChange={(value) => onChange(value, j)} />
      ))}
    </tr>
  )
}, arePropsEqual)

function arePropsEqual(oldProps: RowProps, newProps: RowProps) {
  return oldProps.row === newProps.row
}
