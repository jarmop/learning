import { memo } from 'react'

interface CellProps {
  value: string
  onChange: (value: string) => void
}

export const MemoizedCell = memo(({ value, onChange }: CellProps) => {
  return (
    <td>
      <input type="text" value={value} onChange={(event) => onChange(event.target.value)} />
    </td>
  )
}, arePropsEqual)

function arePropsEqual(oldProps: CellProps, newProps: CellProps) {
  return oldProps.value === newProps.value
}
