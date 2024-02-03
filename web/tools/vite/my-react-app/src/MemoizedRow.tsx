import { memo, useEffect, useRef } from 'react'
import { replaceAtIndex } from './util'
import { MemoizedCell } from './MemoizedCell'

interface RowProps {
  row: string[]
  i: number
  onChange: (row: string[]) => void
}

export const MemoizedRow = memo(({ row, i, onChange }: RowProps) => {
  const rowRef = useRef(row)

  useEffect(() => {
    rowRef.current = row
  }, [row])

  return (
    <tr>
      {row.map((col, j) => (
        <MemoizedCell
          key={`${i}-${j}`}
          value={col}
          onChange={(value) => onChange(replaceAtIndex(rowRef.current, j, value))}
        />
      ))}
    </tr>
  )
}, arePropsEqual)

function arePropsEqual(oldProps: RowProps, newProps: RowProps) {
  return oldProps.row === newProps.row
}
