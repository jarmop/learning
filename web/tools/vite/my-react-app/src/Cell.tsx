interface CellProps {
  value: string
  onChange: (value: string) => void
}

export const Cell = ({ value, onChange }: CellProps) => {
  return (
    <td>
      <input type="text" value={value} onChange={(event) => onChange(event.target.value)} />
    </td>
  )
}
