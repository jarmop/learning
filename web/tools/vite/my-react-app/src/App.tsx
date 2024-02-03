import { useState } from 'react'

const initialData: string[][] = new Array(1000).fill(new Array(6).fill('foo'))

function replaceAtIndex<T>(arr: T[], index: number, value: T) {
  return arr.map((oldValue, oldIndex) => (oldIndex === index ? value : oldValue))
}

function App() {
  const [data, setData] = useState(initialData)

  return (
    <>
      <table>
        <tbody>
          {data.map((row, i) => {
            return (
              <tr key={i}>
                {row.map((col, j) => (
                  <td key={`${i}-${j}`}>
                    <input
                      type="text"
                      value={col}
                      onChange={(event) => {
                        const newData = replaceAtIndex(data, i, replaceAtIndex(row, j, event.target.value))
                        setData(newData)
                      }}
                    />
                  </td>
                ))}
              </tr>
            )
          })}
        </tbody>
      </table>
    </>
  )
}

export default App
