import { useEffect, useRef } from 'react'
import { MemoizedCell } from './MemoizedCell'
import { replaceAtIndex } from './util'

interface RowProps {
  row: string[]
  i: number
  onChange: (row: string[]) => void
}

export const Row = ({ row, i, onChange }: RowProps) => {
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
}
